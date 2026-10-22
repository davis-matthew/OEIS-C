/*
 * A365131  G.f. A(x) satisfies A(x) = (1 + x*A(x)*(1 + x*A(x))^3)^2.
 * https://oeis.org/A365131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} binom(2*(n+1), k) * binom(3*k,
 *            n-k) with binom(a,b) = 0 when b > a. This implements the closed
 *            form for A365131
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          // a(0) = 1

    seq_t sum = 0;
    seq_t t = (seq_t)(n + 1);             // (n+1)
    seq_t two_n_plus_two = 2 * t;         // 2*(n+1)

    seq_t B = (seq_t)1;                   // binom(2*(n+1), 0)
    seq_t Cprev = (seq_t)0;               // previous C value for recurrence

    idx_t k_min = (n + 3) / 4;            // smallest k with binom(3k, n-k) non‑zero

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0) {
            seq_t num_factor = two_n_plus_two - (seq_t)(k - 1); // (2*(n+1) - (k-1))
            B = B * num_factor / (seq_t)k; /* B = binom(2*(n+1), k) */
        }

        seq_t Ck = (seq_t)0;
        if (k >= k_min) {
            if (k == k_min) {
                Ck = binom((idx_t)(3 * k), (idx_t)(n - k));
            } else {
                /* Recurrence for binom(3k, n-k) from the previous term */
                seq_t num = Cprev;
                num = num * (seq_t)(3 * k) * (seq_t)(3 * k - 1) * (seq_t)(3 * k - 2);
                num = num * (seq_t)(n - k + 1);

                seq_t denom = (seq_t)(4 * k - n);
                denom = denom * (seq_t)(4 * k - n - 1);
                denom = denom * (seq_t)(4 * k - n - 2);
                denom = denom * (seq_t)(4 * k - n - 3);

                Ck = num / denom;         /* exact division */
            }
            Cprev = Ck;                 /* store for next iteration */
            sum += B * Ck;
        }
    }

    return sum / t;                       /* divide by (n+1) */
}
