# Arduino Menu Library
This is a menu library for the ssd1306 0.94 inch display.

## Usage
### Menu
to create a menu instance you can easily just:
```cpp
#include "Menu.h"

Menu newMenu; // Creates a new Menu instance
```

to initialize the menu you can call the begin function as following
```cpp
bool success = newMenu.begin();

if (!success) {
  Serial.println("failed to initialized");
}
```

to controll your position on the menu you can call these functions\
```cpp
newMenu.up(); // Moves you up in the menu
newMenu.down(); // Moves you down in the menu
newMenu.right(); // Activates the currectly selected item
newMenu.left(); // Activates the currectly selected item
```

You can also manually refresh the display, it will automatically update the display when you set the page or move in the menu
```cpp
newMenu.refresh();
```

### Pages
```cpp
Page page; // Creates a new Page instance
Page settings;
```

to display a page to the menu you can use the setPage function
```cpp
newMenu.setPage(page);
```

### Menu Items
Menu Items are the items you can add to a page to give your menu a interface.

### Basics
Basic functions for all MenuItems:

Add MenuItem to a page
```cpp
page.addItem(&menuItem);
page.addItem(&secondMenuItem);
page.addItem(&thirdMenuItem);

// or

page.addItem(&menuItem)
    -> addItem(&secondMenuItem)
    -> addItem(&thirdMenuItem);
```

Set title function for a menu item is used to set the item's title
```cpp
menuItem.setTitle("Hello World!");
```

To disable the MenuItem so that the user cannot activate it you can use
```cpp
menuItem.enabled = false;

menuItem.enabled = true;
```

### Label
The label item is used to display a text or value to the menu.

```cpp
Label label("Hello World!");

page.addItem(&label);
```

### Button
The button item is used to have a callback called when the user activates it.

```cpp
void Callback() {
  Serial.println("Button Clicked");
}

Button button("Hello World!", &Callback);

// Or

Button button("Hello World!", [](){
  Serial.println("Button Clicked");
});
```

### Checkbox
the checknox is used to toggle a value or display a value inside of the menu

```cpp
Checkbox checkbox("Checkbox!");

// Initialize with set state

Checkbox checkbox("Checkbox!", true);

// Initialize with callback

void Callback(bool state) {
  Serial.print("Checkbox toggled: ");
  Serial.println(state);
}

Checkbox checkbox("Checkbox!", true, &Callback);
Checkbox checkbox("Checkbox!", &Callback);
```

You can also manually change the value of a checkbox using the setState or toggleState function
```cpp
checkbox.setState(false);
checkbox.setState(true);

checkbox.toggleState();
```

In the same way you can get the state of the checkbox using
```cpp
bool state = checkbox.getState();
```

### NumberInput
The numberinput is a MenuItem where you can set a number or display a number

```cpp
uint8_t default_value = 1;
uint8_t min_value = 1;
uint8_t max_value = 10;

NumberInput numberInput("NumberInput", default_value, min_value, max_value);

// You can also do the same but with a callback

void Callback(uint8_t value) {
  Serial.print("NumberInput changed: ");
  Serial.println(value);
}

NumberInput numberInput("NumberInput", default_value, min_value, max_value, &Callback);
```

To change the value of the NumberInput you can use the following functions
```cpp
numberInput.increase(1);
numberInput.decrease(1);
numberInput.setValue(5);
```
