/*
 * A389717  a(n) = Sum_{k=0..floor(4*n/11)} binomial(k,4*n-11*k).
 * https://oeis.org/A389717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 102, u64 n <= 194,
 *            u128 n <= 377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A389717
   Formula: a(n) = Sum_{k=0..floor(4*n/11)} binomial(k, 4*n-11*k)
   Recurrence: a(n) = 4*a(n-3) - 6*a(n-6) + 4*a(n-9) + a(n-11) - a(n-12),
   with initial values:
   a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0, a(6)=1,
   a(7)=0, a(8)=0, a(9)=1, a(10)=0, a(11)=1
   (Offset 0)
   The recurrence produces non‑negative intermediate sums for all n ≥ 12,
   so unsigned arithmetic is safe. */

#include <stdlib.h>

static const seq_t _init[12] = {
    1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 1
};

seq_t a(idx_t n)
{
    if (n < 12) {
        return _init[n];
    }

    /* Allocate array for terms 0..n (inclusive) */
    seq_t *buf = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!buf) {
        /* Should never happen for reasonably sized n */
        return 0;
    }

    /* Copy the first 12 known terms */
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = _init[i];

    /* Compute remaining terms using the linear recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        /* a(i) = 4*a(i-3) - 6*a(i-6) + 4*a(i-9) + a(i-11) - a(i-12) */
        buf[i] = 4 * buf[i - 3]
                - 6 * buf[i - 6]
                + 4 * buf[i - 9]
                +    buf[i -11]
                -    buf[i -12];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
