/*
 * A386394  a(n) = (5^(2*n) + 3*2^(5*n-2))/7.
 * https://oeis.org/A386394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(2*n) + 3*2^(5*n-2))/7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=7, a(2)=199, a(n)=57*a(n-1)-800*a(n-2) for n>2 */
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 1; return 0 as a fallback */
        return (seq_t)0;
    }

    /* Handle the first two terms directly */
    if (n == 1) {
        return (seq_t)7;
    }
    if (n == 2) {
        return (seq_t)199;
    }

    /* Compute using the linear recurrence.
       a1 = a(n-2), a2 = a(n-1) */
    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)199;
    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t t57  = mul_small(a2, 57);   /* 57*a(n-1)   */
        seq_t t800 = mul_small(a1, 800);  /* 800*a(n-2)  */
        next = t57 - t800;                /* a(n) = 57*a(n-1) - 800*a(n-2) */
        a1 = a2;
        a2 = next;
    }
    return a2;
}
