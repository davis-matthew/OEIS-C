/*
 * A392401  a(n) = Sum_{k=0..floor(2*n/5)} binomial(2*n-2*k,3*k).
 * https://oeis.org/A392401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(2*n/5)} binomial(2*n-2*k,3*k) Recurrence:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=5, a(4)=21 a(n) = 3*a(n-1) -
 *            3*a(n-2) + 7*a(n-3) + 2*a(n-4) + a(n-5)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 832 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* handle initial terms directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 5;
    if (n == 4) return 21;

    /* iterate using the recurrence */
    seq_t p0 = 1;  /* a(n-5) */
    seq_t p1 = 1;  /* a(n-4) */
    seq_t p2 = 1;  /* a(n-3) */
    seq_t p3 = 5;  /* a(n-2) */
    seq_t p4 = 21; /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute each term of the recurrence */
        seq_t t1 = mul_small(p4, 3); /* 3*a(n-1) */
        seq_t t2 = mul_small(p3, 3); /* 3*a(n-2) */
        seq_t t3 = mul_small(p2, 7); /* 7*a(n-3) */
        seq_t t4 = mul_small(p1, 2); /* 2*a(n-4) */
        seq_t sum = t1 + t3 + t4 + p0; /* 3*a(n-1) + 7*a(n-3) + 2*a(n-4) + a(n-5) */
        next = sum - t2;               /* subtract 3*a(n-2) */

        /* shift for next iteration */
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = next;
    }

    return p4;
}
