/*
 * A376582  Triangle of generalized Stirling numbers.
 * https://oeis.org/A376582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(1, r, k) where * T(1, r, k) = Σ_{i=0..r-k} (-1)^i *
 *            C(r+2, r-k-i) * (r+1-k)!/(i+1) * The indices r (row) and k
 *            (column) are obtained from the linear index n * by n = r(r+1)/2
 *            + k, 0 ≤ k ≤ r
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, factorial, idx_to_rowcol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"   /* factorial(idx_t) */
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include "../utils/idx_to_rowcol.h"

/* Compute integer square root of a 64‑bit unsigned integer using only
 * integer arithmetic.  Result is floor(sqrt(x)). */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, k;
    idx_to_rowcol(n, &r, &k);

    /* The factorial (r+1-k)! is needed for all terms. */
    seq_t fact_term = factorial(r + 1 - k);

    /* Accumulate in a signed 128‑bit integer to allow intermediate
     * subtraction without overflow or wrapping. */
    __int128 sum = 0;

    /* Loop over i = 0 … r-k. */
    for (idx_t i = 0; i <= r - k; ++i) {
        /* binomial coefficient C(r+2, r-k-i) */
        seq_t bin = binomial(r + 2, r - k - i);

        /* term = bin * fact_term / (i+1) */
        seq_t num = bin * fact_term;
        seq_t term = num / ((seq_t)(i + 1));

        /* Incorporate alternating sign. */
        if (i & 1)
            sum -= (__int128)term;
        else
            sum += (__int128)term;
    }

    return (seq_t)sum;
}
