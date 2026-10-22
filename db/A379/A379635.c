/*
 * A379635  Triangle read by rows: T(n,k) = A000203(k)*A000203(n-k+1), n >= 1,
 *          k >= 1.
 * https://oeis.org/A379635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = σ(k) * σ(n - k + 1) * where k = n - (r-1)*r/2 and r = the
 *            smallest integer with r*(r+1)/2 ≥ n * σ(m) is the sum of
 *            divisors of m (A000203)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* helper prototypes */
static idx_t find_row(idx_t n);
static seq_t sigma_idx(idx_t m);

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n<1 */

    /* locate the triangle row and column */
    idx_t r = find_row(n);
    idx_t prev = (r - 1) * r / 2;           /* (r-1)*r/2 fits in 64‑bit */
    idx_t k = n - prev;                     /* 1 ≤ k ≤ r */

    /* compute σ(k) and σ(r-k+1) and return their product */
    seq_t s1 = sigma_idx(k);
    seq_t s2 = sigma_idx(r - k + 1);
    return s1 * s2;
}

/* ------------------------------------------------------------------ */
/* Find the smallest r such that r*(r+1)/2 ≥ n (binary search).       */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 2;

    /* grow high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Compute the sum of divisors σ(m) for a small integer m (m ≤ r ≤ 2√n). */
static seq_t sigma_idx(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}
