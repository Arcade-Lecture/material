#include <stdio.h>

// ---------------------------------------------------------------------------------------------------------------------
// C O N F I G U R A T I O N   F L A G S
// ---------------------------------------------------------------------------------------------------------------------

#define ENABLE_PRINTER          // comment this line, if you want to turn off this option
#define COMPILER_PACK_STRUCT    // comment this line, if you want to turn off this option

// ---------------------------------------------------------------------------------------------------------------------
// C O N D I T I O N A L   M A C R O S
// ---------------------------------------------------------------------------------------------------------------------

// depending on whether 'ENABLE_PRINTER' is defined, a macro 'PRINT(text)' delegates 'text' to a print function or not
#ifdef ENABLE_PRINTER
// 'ENABLE_PRINTER' is defined, use 'printf' to output the text
#define PRINT(text)      \
    printf("Printer: "); \
    printf(text)
#else
// 'ENABLE_PRINTER' is not defined; ignore 'text'
#define PRINT()
#endif

// depending on whether 'COMPILER_PACK_STRUCT' is defined, a compiler directive to tightly pack structures is used or not
#ifdef COMPILER_PACK_STRUCT
#pragma pack(push, 1)
#endif

struct structure
<%
    char a;
    int b;
    short c;
%>;

#ifdef COMPILER_PACK_STRUCT
#pragma pack(pop)
#endif

main() {
    // this prints "Hello World" if 'ENABLE_PRINTER' is defined
    PRINT("Hello World\n");

    // if 'COMPILER_PACK_STRUCT' is defined, prints
    //      'size in byte for structure: 7'
    // otherwise, prints
    //      'size in byte for structure: 12'
    printf("size in byte for structure: %d\n", sizeof(struct structure));
}