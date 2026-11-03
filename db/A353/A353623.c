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
 * Validated: standard, pass, 101 terms through n = 9872 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute least k>0 such that n and k*n have no common non‑zero digit
   in balanced ternary.  Brute‑force search.  */
#include <stdbool.h>

/* Return the least k > 0 such that the balanced ternary digits of n
   and k*n never have the same non‑zero digit in the same position. */
static int next_balanced_digit(seq_t *x)
{
    seq_t val = *x;
    seq_t r   = val % (seq_t)3;          /* 0,1,2  */
    if (r == (seq_t)2) {                /* digit –1  */
        *x = (val + (seq_t)1) / (seq_t)3;
        return -1;
    } else {                            /* digit 0 or +1  */
        *x = val / (seq_t)3;
        return (int)r;
    }
}

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
