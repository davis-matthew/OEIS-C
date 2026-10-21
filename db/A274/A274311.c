/*
 * A274311  a(n) = 15*binomial(n,6)-6*binomial(n-2,4)+binomial(n-4,4).
 * https://oeis.org/A274311
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*binomial(n,6) - 6*binomial(n-2,4) + binomial(n-4,4)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial(n,k) for small k (k <= n), using multiplicative formula.
   For n < k the result is 0.  The function tolerates huge intermediate values
   because seq_t is an arbitrary‑precision integer type on the big build. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (n < k)
        return (seq_t)0;
    if (k > n - k)           /* Use symmetry C(n,k)=C(n,n-k) to minimise loops */
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* Multiply first, then divide.  The division is exact at each step. */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)4)          /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    seq_t result = (seq_t)0;

    /* term 1 : 15 * C(n,6) */
    seq_t c1 = binom_small(n, (idx_t)6);
    result += (seq_t)15 * c1;

    /* term 2 : - 6 * C(n-2,4)   (only for n >= 6) */
    if (n >= (idx_t)6) {
        seq_t c2 = binom_small(n - (idx_t)2, (idx_t)4);
        result -= (seq_t)6 * c2;
    }

    /* term 3 : + C(n-4,4)   (only for n >= 8) */
    if (n >= (idx_t)8) {
        seq_t c3 = binom_small(n - (idx_t)4, (idx_t)4);
        result += c3;
    }

    return result;
}
