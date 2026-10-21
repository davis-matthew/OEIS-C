/*
 * A256440  Regular triangle where the n-th row lists the integers k between 1
 *          and n ordered by increasing value of sigma(k)/k where sigma is the
 *          sum of divisors, A000203.
 * https://oeis.org/A256440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the element at position n in the regular triangle where
 *            the k‑th row lists the integers 1 … k sorted by increasing value
 *            of sigma(k)/k (sigma = sum of divisors)
 * Range:     u8 n <= 67, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- Helpers ----- */

/* Cache for divisor sums (1‑based). */
static seq_t *sigma_cache = NULL;
static idx_t max_sigma_computed = 0;

/* Ensure sigma_cache contains sigma(i) for all 1 ≤ i ≤ upTo.  */
static void ensure_sigma(idx_t upTo)
{
    if (upTo <= max_sigma_computed)
        return;                           /* already computed */

    /* Allocate or grow the cache. */
    seq_t *tmp = (seq_t *)realloc(sigma_cache,
                                  (size_t)(upTo + 1) * sizeof(seq_t));
    if (!tmp) {                            /* allocation failed */
        sigma_cache = NULL;
        max_sigma_computed = 0;
        return;
    }
    sigma_cache = tmp;

    /* Compute divisor sums for 1 … upTo using a classic sieve. */
    for (idx_t i = 1; i <= upTo; ++i)
        sigma_cache[i] = 0;                /* reset */

    for (idx_t d = 1; d <= upTo; ++d) {
        for (idx_t j = d; j <= upTo; j += d)
            sigma_cache[j] += d;           /* add divisor d to j */
    }

    max_sigma_computed = upTo;
}

/* Comparator for sorting numbers 1 … m by sigma(i)/i increasing,
   ties broken by the integer value itself. */
static int cmp_sigma(const void *pa, const void *pb)
{
    const idx_t a = *(const idx_t *)pa;
    const idx_t b = *(const idx_t *)pb;

    /* Compare sigma(a)/a with sigma(b)/b by cross‑multiplication. */
    seq_t left  = sigma_cache[a] * b;
    seq_t right = sigma_cache[b] * a;

    if (left < right) return -1;
    if (left > right) return  1;

    /* Ratios equal: order by numeric value. */
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;                /* a == b (should not happen) */
}

/* ----- Main sequence function ----- */

seq_t a(idx_t n)
{
    /* Determine the row m such that T_{m-1} < n ≤ T_m (T_m = m(m+1)/2). */
    idx_t lo = 1, hi = 1;
    while ((hi * (hi + 1)) / 2 < n)
        hi <<= 1;                           /* exponential search upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    /* Index within the row (0‑based). */
    idx_t prev_tri = (row - 1) * row / 2;
    idx_t pos = n - prev_tri - 1;

    /* Cache sigma values up to the current row. */
    ensure_sigma(row);

    /* Build the row array 1 … row. */
    idx_t *nums = (idx_t *)malloc((size_t)row * sizeof(idx_t));
    if (!nums) return 0;                    /* allocation failure fallback */
    for (idx_t i = 0; i < row; ++i)
        nums[i] = i + 1;

    /* Sort the row according to the sigma(i)/i ratio. */
    qsort(nums, (size_t)row, sizeof(idx_t), cmp_sigma);

    /* Retrieve the requested element and clean up. */
    seq_t result = (seq_t)nums[pos];
    free(nums);
    return result;
}
