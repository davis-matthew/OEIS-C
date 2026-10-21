/*
 * A244615  G.f.: Sum_{n>=0} (2 + x^n)^n * x^n / (1-x)^(n+1).
 * https://oeis.org/A244615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A244615:
   Formula: a(n) = 1 + Σ_{m=1..n} Σ_{j=0..min(m,⌊(n-m)/m⌋)} binomial(m, j) *
                 2^(m-j) * binomial(n - j*m, m)
   The function uses the recurrence following directly from the
   generating‑function expansion described in the OEIS entry.
*/

#include <stdint.h>

/* Generate binomial coefficient C(n,k) for n,k <= 64-bit integer.
   The algorithm uses the multiplicative formula and always chooses
   the smaller of k and n-k to minimise operations.  The result fits
   into seq_t when we use it in the main algorithm. */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function.  Uses a simple O(n log n) double‑loop
   implementation directly derived from the closed form above. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0) return (seq_t)1;

    seq_t total = (seq_t)1; /* contribution from m = 0 */

    for (idx_t m = 1; m <= n; ++m) {
        /* j runs from 0 to min(m, floor((n-m)/m)) */
        idx_t max_j = (n - m) / m;
        if (max_j > m) max_j = m;

        for (idx_t j = 0; j <= max_j; ++j) {
            /* binomial(m, j) * 2^(m-j) * binomial(n - j*m, m) */
            seq_t term = binom_uint(m, j);
            term = term * ((seq_t)1 << (m - j));          /* 2^(m-j) */
            term = term * binom_uint(n - j * m, m);
            total += term;
        }
    }

    return total;
}
