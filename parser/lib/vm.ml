open Ctypes
open  Foreign

let lib = Dl.dlopen
    ~filename:"../vm.so"
    ~flags:[Dl.RTLD_NOW]

let vm =
  foreign "vm"
    ~from:lib
    (int @-> int @-> int @-> int @-> int @-> ptr uint8_t @-> int @-> returning void)

let char_pointer_of_int_list ~(l: int list) =
  let arr = CArray.of_list uint8_t (List.map Unsigned.UInt8.of_int l) in
  CArray.start arr
