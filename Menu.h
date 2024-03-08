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

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

class Page;

enum ItemType {
  CORE,
  LABEL,
  BUTTON,
  CHECKBOX,
  NUMBERINPUT,
  PAGEBUTTON,
  SELECTION
};

// 'checkbox_on', 8x8px
const unsigned char bitmap_checkbox_on [] PROGMEM = {
  0xff, 0x81, 0x81, 0x83, 0xc5, 0xa9, 0x91, 0xff
};
// 'checkbox_off', 8x8px
const unsigned char bitmap_checkbox_off [] PROGMEM = {
  0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff
};

class MenuItem {
  private:
    ItemType type = CORE;

  public:
    virtual MenuItem() {};
    char* title;
    bool enabled = true;
    void setTitle(char* Title);
    virtual ItemType getType() { return type; }
    virtual bool getState() {}
    virtual void setState(bool State) {}
    virtual void toggleState() {}
    virtual void invoke() {}
    virtual void increase(uint8_t i) {}
    virtual void decrease(uint8_t i) {}
    virtual void setValue(uint8_t i) {}
    virtual void setMin(uint8_t Min) {}
    virtual void setMax(uint8_t Max) {}
    virtual uint8_t getValue() {}
    virtual Page* getPage() {}
    virtual void next() {}
    virtual void previous() {}
    virtual uint8_t getIndex() {}
    virtual char* getRaw() {}
    virtual void setIndex(uint8_t i) {}
};

class Label : public MenuItem {
  private:
    ItemType type = LABEL;
    
  public:
    Label(char* Title);
    virtual ItemType getType() override { return type; }
};

class Button : public MenuItem {
  private:
    ItemType type = BUTTON;
    void (*callback)(void);
    
  public:
    Button(char* Title, void (*func)(void));
    virtual ItemType getType() override { return type; }
    virtual void invoke() override;
};

class Checkbox : public MenuItem {
  private:
    ItemType type = CHECKBOX;
    bool state = false;
    void (*callback)(uint8_t);

  public:
    Checkbox(char* Title, bool State);
    Checkbox(char* Title);
    Checkbox(char* Title, bool State, void(*func)(bool));
    Checkbox(char* Title, void(*func)(bool));
    virtual ItemType getType() override { return type; }
    virtual bool getState() override;
    virtual void setState(bool State) override;
    virtual void toggleState() override;
};

class NumberInput : public MenuItem {
  private:
    ItemType type = NUMBERINPUT;
    uint8_t value = 0;
    uint8_t min = 0;
    uint8_t max = 10;
    void (*callback)(uint8_t);

  public:
    NumberInput(char* Title, uint8_t Value, uint8_t Min, uint8_t Max);
    NumberInput(char* Title, uint8_t Value, uint8_t Min, uint8_t Max, void (*func)(uint8_t));
    virtual ItemType getType() override { return type; }
    virtual void increase(uint8_t i) override;
    virtual void decrease(uint8_t i) override;
    virtual void setValue(uint8_t i) override;
    virtual void setMin(uint8_t Min) override;
    virtual void setMax(uint8_t Max) override;
    virtual uint8_t getValue() override;
};

class PageButton : public MenuItem {
  private:
    ItemType type = PAGEBUTTON;
    Page* page;

  public:
    PageButton(char* Title, Page* p);
    virtual ItemType getType() override { return type; }
    virtual Page* getPage() override;
};

class Selection : public MenuItem {
  private:
    ItemType type = SELECTION;
    char* content[10];
    uint8_t index = 0;
    uint8_t max = 0;
    void (*callback)(uint8_t);

  public:
    Selection(char* Title, char* selection[], uint8_t maximum);
    Selection(char* Title, char* selection[], uint8_t maximum, uint8_t i);
    Selection(char* Title, char* selection[], uint8_t maximum, void (*func)(uint8_t));
    Selection(char* Title, char* selection[], uint8_t maximum, uint8_t i, void (*func)(uint8_t));
    virtual ItemType getType() override { return type; }
    virtual void next() override;
    virtual void previous() override;
    virtual uint8_t getIndex() override;
    virtual char* getRaw() override;
    virtual void setIndex(uint8_t i) override;
};

class Page {
  private:
    MenuItem* content[10];

  public:
    Page* addItem(MenuItem* item);
    MenuItem* getItem(uint8_t index);
    uint8_t size = 0;
};


class Menu {
  private:
    Page page;
    uint8_t index = 0;
    uint8_t vindex = 0;
    void update();

  public:
    Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);
    bool begin();
    void refresh();
    void setPage(Page p);
    void up();
    void down();
    void right();
    void left();
};
#endif
