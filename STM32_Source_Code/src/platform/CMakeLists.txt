target_sources(${APP} PRIVATE
                stm32l4xx_hal_msp.c
                stm32l4xx_it.c
                syscalls.c
                sysmem.c
                system_stm32l4xx.c
                )