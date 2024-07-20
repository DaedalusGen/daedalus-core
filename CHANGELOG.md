```sh
src/daedalus-entropia/interpreter/interpreter.hpp
```

  - Added `Flags` typedef
  - Added `RuntimeValueWrapper` composed of
    - A `RuntimeValue` field
    - A `Flags` field representing scope escaping behaviors
    - `returnStatementBefore` (eg. break -> true, return -> false)
  - Added `wrap` method
  - Added `flag_contains` and `flag_remove` methods
  - Replaced `RuntimeValue` with `RuntimeValue`
  - Added `escape_flag` param to `evaluate_scope` method

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

  - Implemented `wrap` method
  - Implemented `flag_contains` and `flag_remove` methods
  - Replaced `RuntimeValue` with `RuntimeValue`
  - Applied `wrap` on every function return
  - Added flags and before statement return logic to `evaluate_scope` method
