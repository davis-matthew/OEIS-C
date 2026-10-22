/*
 * A309798  Sum of the odd parts appearing among the largest parts of the
 *          partitions of n into 4 parts.
 * https://oeis.org/A309798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..⌊n/4⌋} sum_{j=k..⌊(n-k)/3⌋}
 *            sum_{i=j..⌊(n-j-k)/2⌋} * (n-i-j-k) * ((n-i-j-k) mod 2) *
 *            Recurrence (valid for n ≥ 28): * a(n)=
 *            2a(n-1)-2a(n-2)+2a(n-3)-2a(n-4)+2a(n-5)-2a(n-7)+4a(n-8) *
 *            -6a(n-9)+6a(n-10)-6a(n-11)+5a(n-12)-4a(n-13)+4a(n-15) *
 *            -5a(n-16)+6a(n-17)-6a(n-18)+6a(n-19)-4a(n-...
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached values of a(n) */
static size_t cached_cap = 0;      /* number of allocated cache entries */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cached_cap = n + 1;
        return;
    }
    if (cached_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cached_cap = new_cap;
    }
}

static void compute_range(idx_t start, idx_t end)
{
    for (idx_t idx = start; idx <= end; ++idx) {
        seq_t value;
        if (idx < 28) { /* compute directly */
            idx_t nn = idx;
            seq_t sum = 0;
            if (nn >= 4) {
                idx_t kmax = nn / 4;
                for (idx_t k = 1; k <= kmax; ++k) {
                    idx_t jmax = (nn - k) / 3;
                    for (idx_t j = k; j <= jmax; ++j) {
                        idx_t imax = (nn - j - k) / 2;
                        for (idx_t i = j; i <= imax; ++i) {
                            idx_t a = nn - i - j - k;
                            if (a & 1) sum += (seq_t)a;
                        }
                    }
                }
            }
            value = sum;
        } else { /* recurrence */
            value =  2*cache[idx-1] - 2*cache[idx-2] + 2*cache[idx-3] - 2*cache[idx-4]
                    + 2*cache[idx-5] - 2*cache[idx-7] + 4*cache[idx-8] - 6*cache[idx-9]
                    + 6*cache[idx-10] - 6*cache[idx-11] + 5*cache[idx-12] - 4*cache[idx-13]
                    + 4*cache[idx-15] - 5*cache[idx-16] + 6*cache[idx-17] - 6*cache[idx-18]
                    + 6*cache[idx-19] - 4*cache[idx-20] + 2*cache[idx-21] - 2*cache[idx-23]
                    + 2*cache[idx-24] - 2*cache[idx-25] + 2*cache[idx-26] - 2*cache[idx-27]
                    + cache[idx-28];
        }
        cache[idx] = value;
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL || cached_cap <= n) {
        size_t old_cap = cached_cap;
        ensure_cache(n);
        compute_range(old_cap, n);
    }
    return cache[n];
}
