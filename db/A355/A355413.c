/*
 * A355413  Lexicographically earliest infinite sequence of positive numbers
 *          such that, for n>1, a(n) AND a(n-1) is distinct from all previous
 *          AND operations between adjacent terms, where AND is the binary AND
 *          operator.
 * https://oeis.org/A355413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=1. * For n odd (n≥1), a(n)=n. * For n even (n≥2)
 *            let k=n/2. * If k is odd: a(n)&(n-1)=n-1 and a(n)&(n+1)=n+1. *
 *            If k is even: a(n)&(n-1)=n-2 and a(n)&(n+1)=n. * a(n) is the
 *            minimal non‑zero integer satisfying these two equations * (this
 *            minimal integer is never a power of two). This...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* offset 0: a(0) = 0 */
    if (n == 0u) return (seq_t)0;

    /* odd indices are fixed points */
    if (n & 1u) return (seq_t)n;

    /* even indices */
    idx_t   k      = n >> 1;          /* k = n/2 */
    seq_t   prev   = (seq_t)(n - 1);  /* a(n-1) = n-1 (odd) */
    seq_t   next   = (seq_t)(n + 1);  /* a(n+1) = n+1 (odd) */
    seq_t   new_and1, new_and2;

    if (k & 1u) {           /* k is odd */
        new_and1 = (seq_t)(n - 1);
        new_and2 = (seq_t)(n + 1);
    } else {                /* k is even */
        new_and1 = (seq_t)(n - 2);
        new_and2 = (seq_t)(n);
    }

    /* Compute the minimal a(n) that satisfies the two AND constraints.
       The constraints are handled bit‑wise. */
    const idx_t BITS = 8u * sizeof(seq_t);   /* number of bits in seq_t */
    seq_t a_val = 0;

    for (idx_t bit = 0; bit < BITS; ++bit) {
        seq_t bit_prev   = (prev   >> bit) & 1;
        seq_t bit_next   = (next   >> bit) & 1;
        seq_t bit_and1   = (new_and1 >> bit) & 1;
        seq_t bit_and2   = (new_and2 >> bit) & 1;
        seq_t bit_a      = 0;

        if (bit_prev) {
            if (bit_and1 == 0)
                bit_a = 0;                     /* a_i must be 0 */
            else { /* bit_and1 == 1 */
                if (bit_next)
                    bit_a = bit_and2;          /* must match the AND with next */
                else
                    bit_a = 1;                 /* free, choose 1 for minimality */
            }
        } else { /* bit_prev == 0 */
            if (bit_next)
                bit_a = bit_and2;              /* forced by AND with next */
            else
                bit_a = 0;                     /* free, choose 0 for minimality */
        }

        if (bit_a)
            a_val |= ((seq_t)1 << bit);
    }

    return a_val;
}
