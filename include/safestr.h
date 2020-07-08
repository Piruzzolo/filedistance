//
// Created by savo9 on 08/07/2020.
//

#ifndef FILEDISTANCE_SAFESTR_H
#define FILEDISTANCE_SAFESTR_H

// glibc doesn't have strlcpy
#ifdef __GNU_LIBRARY__
#include "../include/safe_str/strlcpy.h"
#endif

#endif //FILEDISTANCE_SAFESTR_H
