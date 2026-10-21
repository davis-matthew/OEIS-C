/*
 * A245411  Number of endofunctions on [n] such that no element has a preimage
 *          of cardinality seven.
 * https://oeis.org/A245411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] (exp(x)-x^7/7!)^n Computed via
 *            binomial-inclusion‑exclusion: a(n) = ∑_{k=0}^{⌊n/7⌋} (-1)^k *
 *            C(n,k) * (n)_{7k} / (7!)^k * (n-k)^{n-7k}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* fast path for n==0 */
    if (n == 0) return (seq_t)1;

    const seq_t FACT7 = (seq_t)5040;           /* 7! = 5040 */
    const seq_t ONE   = (seq_t)1;

    idx_t K = n / 7;                           /* maximum k */
    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    seq_t combC = ONE;     /* C(n,0) = 1 */
    seq_t PF    = ONE;     /* falling factorial (n)_0 = 1 */
    seq_t denom = ONE;     /* (7!)^0 = 1 */

    for (idx_t k = 0; k <= K; ++k) {
        if (k > 0) {
            /* Update binomial C(n,k) */
            combC = combC * (seq_t)(n - k + 1) / (seq_t)k;

            /* Update falling factorial (n)_{7k} by multiplying next 7 numbers */
            for (int j = 1; j <= 7; ++j) {
                PF *= (seq_t)(n - 7 * (k - 1) - j + 1);
            }

            /* Update denominator (7!)^k */
            denom *= FACT7;
        }

        /* Compute (n-k)^(n-7k) */
        seq_t power = pow_seq((seq_t)(n - k), (idx_t)(n - 7 * k));

        /* term = C(n,k) * (n)_{7k} / (7!)^k * (n-k)^{n-7k} */
        seq_t term = combC * PF / denom * power;

        if ((k & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    return sum_pos - sum_neg;
}
