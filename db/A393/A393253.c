/*
 * A393253  Manhattan distance from 0 to n in perfect shuffle array of base-2
 *          expansions.
 * https://oeis.org/A393253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n) = 2*a(n) * a(4n+1) = a(4n+2) = 2*a(n) - a(2n) + a(2n+1) *
 *            a(4n+3) = 2*a(n) - 2*a(2n) + 2*a(2n+1) * a(0) = 0, a(1) = 1 *
 *            This implementation uses a simple top‑down recursion * that
 *            evaluates each sub‑term exactly once when needed
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases – the sequence starts with 0,1,1,2,2,... */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    idx_t q = n / 4;
    idx_t r = n % 4;

    if (r == 0) {
        seq_t v = a(q);
        return v + v;          /* 2*a(q) */
    }
    else if (r == 1 || r == 2) {
        seq_t v1 = a(q);             /* a(n/4)          */
        seq_t v2 = a(2 * q);          /* a(2n/4)         */
        seq_t v3 = a(2 * q + 1);      /* a(2n/4+1)       */
        seq_t t  = v1 + v1;           /* 2*a1            */
        return t - v2 + v3;           /* 2*a1 - a2 + a3  */
    }
    else { /* r == 3 */
        seq_t v1 = a(q);                    /* a(n/4)          */
        seq_t v2 = a(2 * q);                /* a(2n/4)         */
        seq_t v3 = a(2 * q + 1);            /* a(2n/4+1)       */
        seq_t t  = v1 + v1;                 /* 2*a1            */
        seq_t t2 = v2 + v2;                 /* 2*a2            */
        seq_t t3 = v3 + v3;                 /* 2*a3            */
        return t - t2 + t3;                /* 2*a1 - 2*a2 + 2*a3 */
    }
}
