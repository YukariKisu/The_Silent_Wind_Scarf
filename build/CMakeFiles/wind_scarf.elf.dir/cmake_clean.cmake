file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "project_elf_src_esp32.c"
  "wind_scarf.bin"
  "wind_scarf.map"
  "CMakeFiles/wind_scarf.elf.dir/project_elf_src_esp32.c.obj"
  "CMakeFiles/wind_scarf.elf.dir/project_elf_src_esp32.c.obj.d"
  "project_elf_src_esp32.c"
  "wind_scarf.elf"
  "wind_scarf.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/wind_scarf.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
