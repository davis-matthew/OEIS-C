/*
 * A331619  a(n) is the smallest positive number k such that the decimal
 *          expansion of n*2^k contains the string n.
 * https://oeis.org/A331619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_mul, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the smallest positive k such that the decimal expansion
   of n*2^k contains the decimal string representation of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../utils/seq_mul.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 specially – 0*2^k is always 0, and 0 contains "0". */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Decimal representation of n – small, fits in a 64‑bit string. */
    char nstr[32];
    snprintf(nstr, sizeof nstr, "%llu", (unsigned long long)n);

    /* Buffer large enough for the decimal form of n*2^k during the search. */
    char buf[8192];
    size_t len;

    /* Current value of n*2^k; start with n*2^0 = n. */
    seq_t cur = (seq_t)n;

    idx_t k = 1;
    for (;;) {
        /* Multiply by 2 : cur = n*2^k */
        cur = seq_mul(cur, (seq_t)2);

        /* Convert current value to decimal string. */
        seq_to_str(cur, buf, &len);
        buf[len] = '\0';

        /* Check whether the decimal string of n occurs as a substring. */
        if (strstr(buf, nstr) != NULL) {
            return (seq_t)k;
        }
        ++k;
    }
}
