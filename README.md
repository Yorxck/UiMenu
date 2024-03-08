# Arduino Menu Library
This is a menu library for the ssd1306 0.94 inch display.

## Usage
### Menu
to create a menu instance you can easily just:
```cpp
  #include "Menu.h"

  Menu newMenu; // Creates a new menu instance
```

to initialize the menu you can call the begin function as following
```cpp
  bool success = menu.begin();

  if (!success) {
    Serial.println("failed to initialized");
  }
```
