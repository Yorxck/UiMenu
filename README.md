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

### Label
The label item is used to display 
