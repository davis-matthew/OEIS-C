/*
 * A270081  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 62", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-5) + 2*a(n-7) -
 *            a(n-8) (n >= 13) The first 13 values are derived from the
 *            numerator of the generating function: a(0)=1, a(1)=6, a(2)=18,
 *            a(3)=38, a(4)=70, a(5)=114, a(6)=182, a(7)=254, a(8)=354,
 *            a(9)=466, a(10)=622, a(11)=782, a(12)=986
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = 0;
static bool init_done  = false;

/* numerator coefficients of the rational generating function, indexed 0..12 */
static const int p_num[13] = {
    1, 4, 6, 4, 4, 0, 6, -12, -1, 4, 4, 0, -4
};

static void ensure_cache_cap(idx_t n)
{
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 64) cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (cache_cap <= n) {
        idx_t new_cap = cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) ensure_cache_cap(0);
    ensure_cache_cap(n);

    /* initialise the first 13 terms only once */
    if (!init_done) {
        cache[0] = (seq_t)1;
        last_computed = 0;
        idx_t upto = n < 12 ? n : 12;
        for (idx_t i = 1; i <= upto; ++i) {
            int64_t sum = (int64_t)p_num[i];
            if (i >= 1) sum += 2LL * (int64_t)cache[i-1];
            if (i >= 3) sum -= 2LL * (int64_t)cache[i-3];
            if (i >= 4) sum += 2LL * (int64_t)cache[i-4];
            if (i >= 5) sum -= 2LL * (int64_t)cache[i-5];
            if (i >= 7) sum += 2LL * (int64_t)cache[i-7];
            if (i >= 8) sum -= (int64_t)cache[i-8];
            cache[i] = (seq_t)sum;
            last_computed = i;
        }
        init_done = true;
        if (last_computed >= n) return cache[n];
    }

    if (last_computed >= n) return cache[n];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 12) {
            int64_t sum = (int64_t)p_num[i];
            if (i >= 1) sum += 2LL * (int64_t)cache[i-1];
            if (i >= 3) sum -= 2LL * (int64_t)cache[i-3];
            if (i >= 4) sum += 2LL * (int64_t)cache[i-4];
            if (i >= 5) sum -= 2LL * (int64_t)cache[i-5];
            if (i >= 7) sum += 2LL * (int64_t)cache[i-7];
            if (i >= 8) sum -= (int64_t)cache[i-8];
            cache[i] = (seq_t)sum;
        } else {
            seq_t pos = cache[i-1] * (seq_t)2;
            pos += cache[i-4] * (seq_t)2;
            pos += cache[i-7] * (seq_t)2;

            seq_t neg = cache[i-3] * (seq_t)2;
            neg += cache[i-5] * (seq_t)2;
            neg += cache[i-8];

            cache[i] = pos - neg;
        }
        last_computed = i;
    }
    return cache[n];
}
