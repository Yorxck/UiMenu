/*
MIT License

Copyright (c) 2024 Yorick

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Menu.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

// Menu
bool Menu::begin() {
  return display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void Menu::update() {
  display.clearDisplay();

  for (uint8_t i = 0; i < constrain(page.size, 0, 4); i++) {
    MenuItem* item = page.getItem(index - vindex + i);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    int16_t x, y;
    uint16_t w, h;
    display.getTextBounds(item->title, 0, 0, &x, &y, &w, &h);

    display.setCursor(((display.height() / 4 - h) / 2), (display.height() / 4 * i) + ((display.height() / 4 - h) / 2));
    display.write(item->title);
    
    switch (item->getType()) {
      case BUTTON: case PAGEBUTTON: 
          display.getTextBounds(">", 0, 0, &x, &y, &w, &h);
          display.setCursor( display.width() - w - ((display.height() / 4 - h) / 2), (display.height() / 4 * i) + ((display.height() / 4 - h) / 2));
          display.write(">");
        break;
      case CHECKBOX:
          //display.getTextBounds( item->getState() ? "ON" : "OFF", 0, 0, &x, &y, &w, &h);
          //display.setCursor( display.width() - w - ((display.height() / 4 - h) / 2), (display.height() / 4 * i) + ((display.height() / 4 - h) / 2));
          //display.write( item->getState() ? "ON" : "OFF");
          display.drawBitmap(display.width() - 8 - ((display.height() / 4 - 8) / 2), (display.height() / 4 * i) + ((display.height() / 4 - 8) / 2), item->getState() ? bitmap_checkbox_on : bitmap_checkbox_off, 8, 8, SSD1306_WHITE);
        break;
      case NUMBERINPUT:
          char buffer[3];
          sprintf(buffer, "%d", item->getValue());
          
          display.getTextBounds(buffer, 0, 0, &x, &y, &w, &h);
          display.setCursor( display.width() - w - ((display.height() / 4 - h) / 2), (display.height() / 4 * i) + ((display.height() / 4 - h) / 2));
          display.write(buffer);
        break;
      case SELECTION:
          display.getTextBounds( item->getRaw(), 0, 0, &x, &y, &w, &h);
          display.setCursor( display.width() - w - ((display.height() / 4 - h) / 2), (display.height() / 4 * i) + ((display.height() / 4 - h) / 2));
          display.write( item->getRaw());
        break;
    }
  }

  display.fillRect(0, display.height() / 4 * vindex, display.width(), display.height() / 4, SSD1306_INVERSE);
  
  display.display();
}

void Menu::refresh() {
  update();
}

void Menu::setPage(Page p) {
  page = p;
  vindex = 0;
  index = 0;

  update();
}

void Menu::up() {
  vindex = constrain(vindex - 1, 0, page.size - 1 < 3 ? page.size - 1 : 3);
  index = constrain(index - 1, 0, page.size - 1);

  update();
}

void Menu::down() {
  vindex = constrain(vindex + 1, 0, page.size - 1 < 3 ? page.size - 1 : 3);
  index = constrain(index + 1, 0, page.size - 1);

  update();
}

void Menu::right() {
  MenuItem* item = page.getItem(index);

  if (!item->enabled) return;
  
  switch (item->getType()) {
    case BUTTON:
      item->invoke();
      break;
    case CHECKBOX:
      item->toggleState();
      break;
    case NUMBERINPUT:
      item->increase(1);
      break;
    case PAGEBUTTON:
      setPage(*item->getPage());
      break;
    case SELECTION:
      item->next();
      break;
  }

  update();
}

void Menu::left() {
  MenuItem* item = page.getItem(index);

  if (!item->enabled) return;
  
  switch (item->getType()) {
    case NUMBERINPUT:
      item->decrease(1);
      break;
    case SELECTION:
      item->previous();
      break;
  }

  update();
}


// Page
Page* Page::addItem(MenuItem* item) {
  if (size >= 10) return;

  content[size] = item;
  size++;

  return this;
}

MenuItem* Page::getItem(uint8_t index) {
  return content[index];
}

// MenuItem
void MenuItem::setTitle(char* Title) {
  title = Title;
}

// Label
Label::Label(char* Title) {
  title = Title;
}

// Button
Button::Button(char* Title, void (*func)(void) ) {
  title = Title;
  callback = func;
}

void Button::invoke() {
  callback();
}

// Checkbox
Checkbox::Checkbox(char* Title, bool State) {
  title = Title;
  state = State;
}

Checkbox::Checkbox(char* Title) {
  title = Title;
}

Checkbox::Checkbox(char* Title, bool State, void (*func)(bool)) {
  title = Title;
  state = State;
  callback = func;
}

Checkbox::Checkbox(char* Title, void (*func)(bool)) {
  title = Title;
  callback = func;
}

bool Checkbox::getState() {
  return state;
}

void Checkbox::toggleState() {
  state = !state;

  if (callback != NULL) {
    callback(state);
  }
}

void Checkbox::setState(bool State) {
  state = State;

  if (callback != NULL) {
    callback(state);
  }
}

// NumberInput
NumberInput::NumberInput(char* Title, uint8_t Value, uint8_t Min, uint8_t Max) {
  title = Title;
  min = Min;
  max = Max;
  value = constrain(Value, min, max);
}

NumberInput::NumberInput(char* Title, uint8_t Value, uint8_t Min, uint8_t Max, void (*func)(uint8_t)) {
  title = Title;
  min = Min;
  max = Max;
  callback = func;
  value = constrain(Value, min, max);
}

void NumberInput::increase(uint8_t i) {
  value = constrain(value+i, min, max);

  if (callback != NULL) {
    callback(value);
  }
}

void NumberInput::decrease(uint8_t i) {
  value = constrain(value-i, min, max);

  if (callback != NULL) {
    callback(value);
  }
}

void NumberInput::setValue(uint8_t i) {
  value = constrain(i, min, max);

  if (callback != NULL) {
    callback(value);
  }
}

void NumberInput::setMin(uint8_t Min) {
  min = Min;
}

void NumberInput::setMax(uint8_t Max) {
  max = Max;
}

uint8_t NumberInput::getValue() {
  return value;
}

// PageButton
PageButton::PageButton(char* Title, Page* p) {
  title = Title;
  page = p;
}

Page* PageButton::getPage() {
  return page;
}

// Selection
Selection::Selection(char* Title, char* selection[], uint8_t maximum) {
  for (uint8_t i = 0; i < maximum; i++) {
    content[i] = selection[i];
  }

  max = maximum;
  title = Title;
}

Selection::Selection(char* Title, char* selection[], uint8_t maximum, uint8_t i) {
  for (uint8_t i = 0; i < maximum; i++) {
    content[i] = selection[i];
  }

  max = maximum;
  title = Title;
  index = i;
}

Selection::Selection(char* Title, char* selection[], uint8_t maximum, void (*func)(uint8_t)) {
  for (uint8_t i = 0; i < maximum; i++) {
    content[i] = selection[i];
  }

  max = maximum;
  callback = func;
  title = Title;
}

Selection::Selection(char* Title, char* selection[], uint8_t maximum, uint8_t i, void (*func)(uint8_t)) {
  for (uint8_t i = 0; i < maximum; i++) {
    content[i] = selection[i];
  }

  max = maximum;
  callback = func;
  title = Title;
  index = i;
}

void Selection::next() {
  index = constrain(index + 1, 0, max-1);

  if (callback != NULL) {
    callback(index);
  }
}

void Selection::previous() {
  index = constrain(index - 1, 0, max-1);
  
  if (callback != NULL) {
    callback(index);
  }
}

uint8_t Selection::getIndex() {
  return index;
}

void Selection::setIndex(uint8_t i) {
  index = constrain(i, 0, max);

  if (callback != NULL) {
    callback(index);
  }
}

char* Selection::getRaw() {
  return content[index];  
}
