/*
 * A289807  p-INVERT of (1,2,2,3,3,4,4,...) (A080513), where p(S) = 1 - S -
 *          S^2.
 * https://oeis.org/A289807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 5*a(n-3) + a(n-4) + 2*a(n-5) -
 *            a(n-6) Initial values: a(0)=1, a(1)=4, a(2)=13, a(3)=42,
 *            a(4)=133, a(5)=424
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)42;
    if (n == 4) return (seq_t)133;
    if (n == 5) return (seq_t)424;

    /* circular buffer holding the last six values:
       a0,a1,a2,a3,a4,a5 correspond to
       a(n-6), a(n-5), a(n-4), a(n-3), a(n-2), a(n-1)  */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t a2 = (seq_t)13;  /* a(2) */
    seq_t a3 = (seq_t)42;  /* a(3) */
    seq_t a4 = (seq_t)133; /* a(4) */
    seq_t a5 = (seq_t)424; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute new value using the recurrence, avoiding overflow
           by multiplying with small constants using mul_small. */
        seq_t pos = mul_small(a5, 3);          /* 3*a(n-1) */
        pos += mul_small(a4, 2);              /* +2*a(n-2) */
        pos += a2;                            /* +a(n-4) */
        pos += mul_small(a1, 2);              /* +2*a(n-5) */

        seq_t neg = mul_small(a3, 5);          /* 5*a(n-3) */
        neg += a0;                            /* +a(n-6) */

        seq_t new_val = pos - neg;            /* final recurrence */

        /* shift the buffer for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = new_val;
    }

    return a5;
}
