/*
 * A272544  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 493", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (2142*4^(n-3) + 196*3^(n-4) - 531*2^(n-3) + 144)/9, n>5
 *            Initial values: a(0)=1, a(1)=8, a(2)=44, a(3)=204, a(4)=876,
 *            a(5)=3652
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring – works for all seq_t types. */
static seq_t pow_seq_expo(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Table of the first six terms. */
static const seq_t initial[] = {
    (seq_t)1,  /* a(0) */
    (seq_t)8,  /* a(1) */
    (seq_t)44, /* a(2) */
    (seq_t)204,/* a(3) */
    (seq_t)876,/* a(4) */
    (seq_t)3652/* a(5) */
};

seq_t a(idx_t n)
{
    if (n <= 5)                /* return one of the pre‑computed values */
        return initial[n];

    /* Exponents for the powers in the formula.  All are non‑negative here. */
    idx_t e4 = n - 3;      /* exponent for 4 */
    idx_t e3 = n - 4;      /* exponent for 3 */
    idx_t e2 = n - 3;      /* exponent for 2 */

    /* Compute the powers with the helper above. */
    seq_t p4 = pow_seq_expo((seq_t)4, e4);
    seq_t p3 = pow_seq_expo((seq_t)3, e3);
    seq_t p2 = pow_seq_expo((seq_t)2, e2);

    /* Combine according to the conjectured formula. */
    seq_t term1 = (seq_t)2142 * p4;
    seq_t term2 = (seq_t)196 * p3;
    seq_t term3 = (seq_t)531 * p2;

    seq_t total = term1 + term2 + (seq_t)144;
    total -= term3;
    total /= (seq_t)9;           /* integer division – the result is exact */

    return total;
}
