#include <stdio.h>
#include <stdlib.h>
#include "structures/types.h"
#include "log/log.h"
#include "string/string.h"
#include "file/file.h"


int main() {
    String string = read_contents("fixtures/test.txt");

    printf("%s", string.data);

    string_dealloc(&string);

    return 0;
}