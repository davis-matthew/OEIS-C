/*
 * A353623  a(n) is the least k > 0 such that n and k*n can be added without
 *          carries in balanced ternary.
 * https://oeis.org/A353623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3992, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_balanced_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute least k>0 such that n and k*n have no common non‑zero digit
   in balanced ternary.  Brute‑force search.  */
#include <stdbool.h>
#include "../utils/next_balanced_digit.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* a(0) = 1 by definition */

    seq_t nn = (seq_t)n;                /* convert the index once */
    idx_t  k  = 1;

    for (;;) {
        seq_t y = nn * (seq_t)k;        /* product n*k            */
        seq_t x = nn;                   /* copy of n for digit   */

        bool ok = true;
        while (x != (seq_t)0 || y != (seq_t)0) {
            int d1 = (x == (seq_t)0) ? 0 : next_balanced_digit(&x);
            int d2 = (y == (seq_t)0) ? 0 : next_balanced_digit(&y);
            if (d1 != 0 && d1 == d2) { /* same non‑zero digit → carry */
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;        /* first successful k */
        ++k;
    }
}
