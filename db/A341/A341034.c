/*
 * A341034  a(n) is the smallest positive integer such that n+a(n) contains
 *          the string n-a(n) as a substring. If no such number exists then
 *          a(n) = -1.
 * https://oeis.org/A341034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive integer a such that the decimal representation
   of n + a contains the decimal representation of n – a as a substring. */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Search for the minimal a (1 ≤ a ≤ n) satisfying the substring condition. */
    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        /* Use unsigned arithmetic for up to 64‑bit values; n and a_val are small. */
        unsigned long long sum  = (unsigned long long)n + a_val;
        unsigned long long diff = (unsigned long long)n - a_val;

        char sumStr[32];
        char diffStr[32];

        /* Convert to decimal strings. */
        snprintf(sumStr,  sizeof sumStr,  "%llu", sum);
        snprintf(diffStr, sizeof diffStr, "%llu", diff);

        /* Check if diffStr occurs in sumStr. */
        if (strstr(sumStr, diffStr) != NULL) {
            return (seq_t)a_val;
        }
    }
    /* No such a exists for this n. */
    return (seq_t)-1;
}
