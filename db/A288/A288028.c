/*
 * A288028  Number of maximal matchings in the grid graph P_3 X P_n.
 * https://oeis.org/A288028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +5*a(n-2) +11*a(n-3) +5*a(n-4) +14*a(n-5) *
 *            +8*a(n-6) +3*a(n-7) -5*a(n-9) -11*a(n-10) -a(n-11) * +2*a(n-12)
 *            +a(n-15) for n > 15 * with a(1) = 2 * a(2) = 5 * a(3) = 22 *
 *            a(4) = 75 * a(5) = 264 * a(6) = 941 * a(7) = 3286 * a(8) =11623
 *            * a(9) =40960 * a(10)=144267 * a(11)=508812 * a(12...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache; index 0 unused */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* initial values 1‑based; cache[0] = 0 */
static const seq_t init[16] = {
    0ULL,          /* dummy 0‑th */
    2ULL, 5ULL, 22ULL, 75ULL, 264ULL,
    941ULL, 3286ULL, 11623ULL, 40960ULL,
    144267ULL, 508812ULL, 1792981ULL, 6319994ULL,
    22277291ULL, 78518760ULL
};

static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    /* allocation / growth */
    if (!cache) {
        cache_cap = n + 1;
        if (cache_cap < 32) cache_cap = 32;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return;              /* OOM – caller gets 0 */
        cache[0] = 0;
        cache_len = 0;
    }

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return;                /* OOM – caller gets 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 15) {
            val = init[i];
        } else {
            val = cache[i - 1];
            val += (seq_t)5 * cache[i - 2];
            val += (seq_t)11 * cache[i - 3];
            val += (seq_t)5 * cache[i - 4];
            val += (seq_t)14 * cache[i - 5];
            val += (seq_t)8 * cache[i - 6];
            val += (seq_t)3 * cache[i - 7];
            val -= (seq_t)5 * cache[i - 9];
            val -= (seq_t)11 * cache[i - 10];
            val -= cache[i - 11];
            val += (seq_t)2 * cache[i - 12];
            val += cache[i - 15];
        }
        cache[i] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* sequence offset is 1 */
    ensure_cache(n);
    if (cache && n <= cache_len) return cache[n];
    return 0;                  /* OOM or other failure */
}
