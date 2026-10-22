/*
 * A373086  Triangle read by rows: T(n, k) is the number of parking functions
 *          of length n with preferences restricted to {1, ..., k} for 0 <= k
 *          <= n.
 * https://oeis.org/A373086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) k = n -
 *            r*(r+1)/2 T(n,k) = Σ_{i=k..n} (-1)^(n-i) * binomial(n,i) *
 *            (i+1)^(i-1) * (i-k+1)^(n-i)
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helpers are provided by the build system: */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static idx_t isqrt_u64(idx_t x)
/* integer floor sqrt using binary search, does not use floating point */
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > x / mid) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

static seq_t triangle_term(idx_t n, idx_t k)
/* Computes T(n,k) for 0 ≤ k ≤ n using the alternating sum formula */
{
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t i = k; i <= n; ++i) {
        seq_t bin   = binomial(n, i);
        seq_t pow1;
        if (i == 0) {
            pow1 = 1;                                    // (i+1)^(i-1) = 1 for i=0
        } else {
            pow1 = big_pow((seq_t)(i + 1), (seq_t)(i - 1));
        }
        seq_t pow2 = big_pow((seq_t)(i - k + 1), (seq_t)(n - i));
        seq_t term = bin * pow1 * pow2;
        if (((n - i) & 1ULL) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;
}

seq_t a(idx_t n)
/* Returns the OEIS sequence A373086 value at index n */
{
    if (n == 0) return 1;

    /* Find row r: minimal r such that r*(r+1)/2 > n */
    idx_t t = 8 * n + 1;
    idx_t s = isqrt_u64(t);
    idx_t r = (s - 1) / 2;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return triangle_term(r, k);
}
