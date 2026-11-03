/* ipow: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_H
#define OEISC_UTILS_IPOW_H
#include <stdbool.h>
#include <stdint.h>

/* Multiply-by-self exponentiation for big exponent (seq_t type) */
static seq_t ipow(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if ((exp % 2) != 0) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

#endif
