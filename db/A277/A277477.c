/*
 * A277477  E.g.f.: -cos(x)*LambertW(-x).
 * https://oeis.org/A277477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=0}^{⌊(n-1)/2⌋} (-1)^m · binom(n,2m) ·
 *            (n-2m)^(n-2m-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   binom, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by the exponential generating function: */
                /* -cos(x)·LambertW(-x) */
                /* a(0)=0, a(1)=1, a(2)=2, ... */

#include "../utils/binom.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* a(0) is the first term */
    if (n == 0) return (seq_t)0;

    /* maximum index m such that n-2m ≥ 1 */
    idx_t max_m = (n - 1) / 2;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t m = 0; m <= max_m; ++m) {
        /* k = 2m is the even index in the cosine series */
        idx_t k = m << 1;                /* same as 2*m */
        seq_t bin   = binom(n, k);       /* binom(n,2m) */
        seq_t pow   = pow_int((seq_t)(n - k), n - k - 1); /* (n-2m)^(n-2m-1) */
        seq_t term  = bin * pow;

        if ((m & 1) == 0) {
            sum_pos += term;
        } else {
            sum_neg += term;
        }
    }

    /* All terms are non‑negative and the sum of positive terms
       dominates, so unsigned subtraction is safe. */
    return sum_pos - sum_neg;
}
