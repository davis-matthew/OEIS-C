/*
 * A340584  Irregular triangle read by rows T(n,k) in which row n lists
 *          sigma(n) + sigma(n-1) together with the first n - 2 terms of
 *          A000203 in reverse order, with T(1,1) = 1, n >= 1.
 * https://oeis.org/A340584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A340584 – irregular triangle read by rows. For row r ≥ 2 the row
 *            consists of sigma(r) + sigma(r-1), followed by sigma(r-2),
 *            sigma(r-3), …, sigma(1) in reverse order. The sequence is
 *            obtained by concatenating these rows, with a(1) = 1
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Sum of divisors (σ) of a positive integer m. */
static seq_t sigma_val(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i) {
                sum += (seq_t)j;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* Find the row r such that the n-th term lies in that row.
       The cumulative number of terms up to row r is
           P(r) = 1 + r(r-1)/2   (for r ≥ 1).
       Thus we need the smallest r with P(r) ≥ n. */
    long double val = 1.0L + 8.0L * (n - 1);   /* 1 + 8*(n-1) */
    long double sqrt_val = sqrtl(val);
    idx_t r = (idx_t)ceill((1.0L + sqrt_val) / 2.0L);

    /* Correct possible off‑by‑one errors from floating point. */
    while (1 + r * (r - 1) / 2 < n) ++r;
    while (r > 1 && 1 + (r - 1) * (r - 2) / 2 >= n) --r;

    /* Position k within row r (1‑based). */
    idx_t prev_sum = 1 + (r - 1) * (r - 2) / 2;   /* P(r-1) */
    idx_t k = n - prev_sum;                       /* 1 ≤ k ≤ r-1 */

    if (k == 1) {
        /* First element of the row: σ(r) + σ(r-1). */
        seq_t first = sigma_val(r);
        seq_t second = sigma_val(r - 1);
        return first + second;
    } else {
        /* Subsequent elements: σ(r-k). */
        return sigma_val(r - k);
    }
}
