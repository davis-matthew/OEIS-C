/*
 * A277478  E.g.f.: -cosh(x)*LambertW(-x).
 * https://oeis.org/A277478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n, (n‑k) even} binomial(n,k) * k^(k‑1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0)=0, a(1)=1, a(2)=2, a(3)=12, a(4)=76, … */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;

    int parity = n & 1;          /* k must have the same parity as n */
    seq_t binom = 1;             /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom to binomial(n,k) using the recurrence:
           binom(n,k) = binom(n,k‑1) * (n‑k+1) / k.           */
        binom = (binom * ((seq_t)(n - k + 1))) / (seq_t)k;

        if ( (k & 1) == parity ) {
            /* Only k with the same parity as n contribute. */
            seq_t powk = pow_int((seq_t)k, k - 1);   /* k^(k‑1) */
            sum += binom * powk;
        }
    }

    return sum;
}
