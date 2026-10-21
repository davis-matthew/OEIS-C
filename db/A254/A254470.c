/*
 * A254470  Sixth partial sums of fourth powers (A000583).
 * https://oeis.org/A254470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + n)*(2 + n)*(3 + n)^2*(4 + n)*(5 + n)*(6 + n)*(1 +
 *            12*n + 2*n^2)/302400 Equivalent recurrence with ${n^4}$ forcing
 *            term: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) +
 *            6*a(n-5) - a(n-6) + n^4 Initial values: a(1)=1, a(2)=22,
 *            a(3)=198, a(4)=1134, a(5)=4884, a(6)=17226
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 27, u64 n <= 275, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed terms (0‑based) */
static idx_t cache_len = 0;          /* number of terms stored */
static idx_t cache_cap = 0;          /* allocated capacity */

/* ensure the cache holds at least 'need' terms (1‑based indexing) */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    /* grow a bit beyond need to reduce reallocations */
    idx_t new_cap = need + 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* assume allocation succeeds; program will crash otherwise */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the first six terms */
static void init_cache(void)
{
    if (cache_len > 0)
        return;
    ensure_cache(8);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)22;
    cache[2] = (seq_t)198;
    cache[3] = (seq_t)1134;
    cache[4] = (seq_t)4884;
    cache[5] = (seq_t)17226;
    cache_len = 6;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined, but harmless */

    init_cache();

    if (n <= cache_len)
        return cache[n - 1];        /* 0‑based */

    /* make room for new terms */
    ensure_cache(n);

    /* compute successive terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        idx_t cur_n = i + 1;        /* n for current term */

        /* recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6) + n^4 */
        unsigned __int128 val = 0;

        val += 6 * (unsigned __int128)cache[i - 1];
        val -= 15 * (unsigned __int128)cache[i - 2];
        val += 20 * (unsigned __int128)cache[i - 3];
        val -= 15 * (unsigned __int128)cache[i - 4];
        val += 6 * (unsigned __int128)cache[i - 5];
        val -= (unsigned __int128)cache[i - 6];

        /* add cur_n^4 */
        unsigned __int128 cn = (unsigned __int128)cur_n;
        unsigned __int128 pow4 = cn * cn * cn * cn;
        val += pow4;

        cache[i] = (seq_t)val;
    }

    cache_len = n;
    return cache[n - 1];
}
