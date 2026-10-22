/*
 * A350158  The distribution of the distance from the first weak subcedance to
 *          1 on permutations.
 * https://oeis.org/A350158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer such that r(r+1)/2
 *            ≥ n and k = n - (r-1)r/2 - 1. T(n,0) = 1 for n=1; T(n,0) =
 *            2*(n-1)! + Σ_{j=1}^{n-2} j^(n−j−1)·j! for n≥2; T(n,k) = (n-1)! –
 *            k^(n−k−1)·k! for 1 ≤ k ≤ n-1
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *fact = NULL;        /* factorial[0..N] */
static idx_t fact_size = 0;       /* current allocated size */

/* Ensure factorials up to n are available */
static void ensure_fact(idx_t n)
{
    if (n < fact_size) return;
    idx_t new_size = (n == 0) ? 1 : n + 1;
    seq_t *tmp = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure: just abort */
    fact = tmp;
    for (idx_t i = fact_size; i <= n; ++i) {
        if (i == 0) fact[i] = (seq_t)1;
        else fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_size = new_size;
}

/* Integer power: base^exp for exp >= 0 */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        if (exp >>= 1)
            base *= base;
    }
    return result;
}

/* Compute T(n,k) using precomputed factorials */
static seq_t T_of(idx_t n, idx_t k)
{
    ensure_fact(n);
    if (k == 0) {
        if (n == 1) return (seq_t)1;
        seq_t res = 2 * fact[n-1];
        for (idx_t j = 1; j <= n-2; ++j) {
            seq_t p = pow_seq((seq_t)j, n - j - 1);
            res += p * fact[j];
        }
        return res;
    } else {
        /* 1 <= k <= n-1 */
        seq_t res = fact[n-1];
        seq_t p = pow_seq((seq_t)k, n - k - 1);
        res -= p * fact[k];
        return res;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but return 0 */

    /* Find row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = 2;
    while ((high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Compute col k = n - (r-1)r/2 - 1 */
    idx_t prev_cum = ((r - 1) * r) / 2;
    idx_t k = n - prev_cum - 1;

    return T_of(r, k);
}
