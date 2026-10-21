/*
 * A257014  Number of (n+2)X(1+2) 0..2 arrays with no 3x3 subblock diagonal
 *          sum equal to the antidiagonal sum.
 * https://oeis.org/A257014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 9*a(n-1) + 2592*a(n-3) +
 *            40824*a(n-4) + 192456*a(n-5) + 1679616*a(n-6) - 90699264*a(n-8)
 *            - 272097792*a(n-9) initial values a(1)..a(9) are listed below.
 *            The function memoises computed terms and works for any n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* first 9 terms of the sequence, used for initialization */
static const seq_t base_terms[9] = {
    15066ULL,
    311364ULL,
    6508512ULL,
    136048896ULL,
    2834352000ULL,
    59049000000ULL,
    1231525944000ULL,
    25684705088064ULL,
    535477332658752ULL
};

/* cache for already computed terms (1‑based indexing → 0‑based array) */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;     /* allocated size in terms */
static idx_t  computed_up_to = 0; /* largest n whose a(n) is stored */

/* initialise the cache with the first nine terms */
static void init_cache(idx_t n)
{
    idx_t size = n > 9 ? n : 9;                      /* at least 9 */
    cache = (seq_t *)malloc(size * sizeof(seq_t));
    if (!cache) abort();
    for (idx_t i = 0; i < 9; ++i) cache[i] = base_terms[i];
    cache_size   = size;
    computed_up_to = 9;
}

/* grow the cache if necessary, keeping the first nine terms initialized */
static void grow_cache(idx_t n)
{
    if (cache_size >= n) return;
    idx_t new_size = cache_size ? cache_size : 9;
    while (new_size < n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = cache_size; i < 9 && i < new_size; ++i) tmp[i] = base_terms[i];
    cache      = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined, but safe value */

    if (!cache) init_cache(n);
    else if (cache_size < n) grow_cache(n);

    if (n <= 9) return cache[n-1];

    if (n > computed_up_to) {
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            idx_t idx = k - 1;              /* array index for a(k) */
            seq_t val = 0;
            val += (seq_t)9        * cache[idx - 1];
            val += (seq_t)2592     * cache[idx - 3];
            val += (seq_t)40824    * cache[idx - 4];
            val += (seq_t)192456   * cache[idx - 5];
            val += (seq_t)1679616  * cache[idx - 6];
            val -= (seq_t)90699264 * cache[idx - 8];
            val -= (seq_t)272097792* cache[idx - 9];
            cache[idx] = val;
        }
        computed_up_to = n;
    }
    return cache[n-1];
}
