/*
 * A286300  Square root of smallest square formed from n by incorporating all
 *          the digits of n in a new decimal number.
 * https://oeis.org/A286300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest k such that k² is the smallest square that
 *            contains all decimal digits of n (with multiplicity). For n
 *            itself a perfect square, a(n) = √n
 * Range:     u8 n <= 6, u16 n <= 1716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Count required digits of n */
    uint8_t need[10] = {0};
    idx_t t = n;
    if (t == 0) {               /* n = 0 (not used in OEIS) */
        need[0] = 1;
    } else {
        while (t > 0) {
            need[t % 10]++;
            t /= 10;
        }
    }

    /* Quick case: n is itself a perfect square */
    seq_t fsqrt = floor_sqrt((seq_t)n);      /* seq_t result */
    idx_t k0     = (idx_t)fsqrt;             /* floor sqrt fits in idx_t */
    if ((seq_t)k0 * (seq_t)k0 == (seq_t)n) {
        return fsqrt;                       /* a(n) = k0 */
    }

    idx_t k = k0 + 1;                       /* start searching */

    for (;;) {
        seq_t sq = (seq_t)k * (seq_t)k;

        /* Count digits of the square */
        uint8_t have[10] = {0};
        seq_t x = sq;
        if (x == 0) {                     /* x = 0 (n = 0) */
            have[0] = 1;
        } else {
            while (x > 0) {
                int d = (int)(x % 10);
                have[d]++;
                x /= 10;
            }
        }

        /* Check whether all required digits are present */
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (have[d] < need[d]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return (seq_t)k;                /* k is the desired root */
        }
        ++k;
    }
}
