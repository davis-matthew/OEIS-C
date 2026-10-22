/*
 * A375037  Irregular triangle read by rows T(n,k), n >= 1, k >= 1, in which
 *          row n lists the divisors of n but every middle divisor is replaced
 *          with zero.
 * https://oeis.org/A375037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n‑th element of the irregular triangle T(m,k) where
 *            * T(m,k) = divisor_k(m) if divisor_k(m) not in [sqrt(m/2),
 *            sqrt(2*m)) * 0 otherwise, and rows are read consecutively
 * Range:     u8 n <= 51, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* integer square root, returns floor(sqrt(n)) */
static idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sq = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            /* never let high underflow */
            if (mid == 0) break;
            else high = mid - 1;
    }
    return high;
}

/* number of divisors of m */
static idx_t divisor_count(idx_t m)
{
    idx_t cnt = 0;
    idx_t r = isqrt_uint64(m);
    for (idx_t i = 1; i <= r; ++i) {
        if (m % i == 0) {
            if (i == m / i) cnt += 1;
            else cnt += 2;
        }
    }
    return cnt;
}

/* return the k-th (0‑based) divisor of m in ascending order */
static unsigned int kth_divisor(idx_t m, size_t k)
{
    idx_t r = isqrt_uint64(m);
    size_t small_alloc = (size_t)r + 1;
    unsigned int *small = (unsigned int*)malloc(small_alloc * sizeof(unsigned int));
    if (!small) return 0;                /* fallback */
    unsigned int *large = (unsigned int*)malloc(small_alloc * sizeof(unsigned int));
    if (!large) { free(small); return 0; }

    size_t s_cnt = 0, l_cnt = 0;
    for (idx_t i = 1; i <= r; ++i) {
        if (m % i == 0) {
            small[s_cnt++] = (unsigned int)i;
            idx_t q = m / i;
            if (q != i) large[l_cnt++] = (unsigned int)q;
        }
    }

    unsigned int d = 0;
    if (k < s_cnt) {
        d = small[k];
    } else {
        size_t idx = k - s_cnt;                /* index within large part */
        d = large[l_cnt - 1 - idx];            /* large part is reversed */
    }

    free(small);
    free(large);
    return d;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out of range guard */

    static idx_t *cum = NULL;                 /* cumulative divisor totals */
    static size_t cum_cap = 0;
    static size_t max_m = 0;

    /* initialize cum array */
    if (!cum) {
        cum_cap = 128;
        cum = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        if (!cum) return 0;                  /* memory exhaustion */
        cum[0] = 0;                          /* cum[0] = 0 for convenience */
    }

    /* extend cumulative totals until they cover n */
    while (cum[max_m] < n) {
        ++max_m;
        if (max_m >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t*)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) return 0;              /* memory exhaustion */
            cum = tmp;
            cum_cap = new_cap;
        }
        idx_t t = divisor_count(max_m);
        cum[max_m] = cum[max_m - 1] + t;
    }

    /* binary search for the row m containing the n-th element */
    size_t lo = 1, hi = max_m;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cum[mid] >= n) hi = mid;
        else lo = mid + 1;
    }
    size_t m = lo;

    /* position within that row (1‑based) */
    idx_t prev_sum = (m == 1) ? 0 : cum[m - 1];
    idx_t pos_in_row = n - prev_sum;
    size_t k_index = (size_t)(pos_in_row - 1);   /* 0‑based index */

    /* get the k-th divisor of m */
    unsigned int d = kth_divisor(m, k_index);

    /* test whether this divisor lies in the forbidden interval */
    seq_t dsq = (seq_t)d * (seq_t)d;
    seq_t twice_dsq = dsq * 2;
    seq_t twice_m = (seq_t)m * 2;
    if (twice_dsq >= (seq_t)m && dsq < twice_m)
        return 0;
    return (seq_t)d;
}
