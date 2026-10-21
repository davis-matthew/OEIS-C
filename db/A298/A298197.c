/*
 * A298197  Number of Eulerian cycles in the graph C_4 X C_n.
 * https://oeis.org/A298197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 242*a(n-1) -24508*a(n-2) +1377576*a(n-3) -48319952*a(n-4)
 *            +1127281504*a(n-5) -18164303168*a(n-6) +206564578176*a(n-7)
 *            -1673816120832*a(n-8) +9654475382784*a(n-9)
 *            -39144748253184*a(n-10) +108479226544128*a(n-11)
 *            -194648732467200*a(n-12) +202715666841600*a(n-13)
 *            -92493840384000*a(n-14), a(1)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const long long coeff[14] = {
    242LL,
   -24508LL,
   1377576LL,
  -48319952LL,
 1127281504LL,
 -18164303168LL,
 206564578176LL,
-1673816120832LL,
 9654475382784LL,
-39144748253184LL,
108479226544128LL,
-194648732467200LL,
202715666841600LL,
-92493840384000LL
};

static seq_t *cache = NULL;
static idx_t max_cached = 0;
static size_t cache_cap = 0;

static void init_cache(void) {
    if (cache != NULL)
        return;
    cache_cap = 15;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        return;
    static const char *const s[15] = {
        NULL,
        "16",
        "2368",
        "207496",
        "15639936",
        "1116199200",
        "77643032832",
        "5318859987584",
        "360460090519552",
        "24225364155392512",
        "1617040095771160576",
        "107318756823774554112",
        "7087408485751290626048",
        "466051677657117523779584",
        "30530955397986792883159040"
    };
    for (idx_t i = 1; i <= 14; ++i)
        cache[i] = seq_from_str(s[i]);
    max_cached = 14;
}

static void ensure_cache_cap(idx_t n) {
    if (cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* offset is 1 */
    init_cache();
    if (n <= max_cached)
        return cache[n];

    ensure_cache_cap(n);

    const bool big = sizeof(seq_t) > 8;
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (!big) {
            /* --- small (≤64‑bit) sequence: use 128‑bit intermediates --- */
            unsigned __int128 pos = 0, neg = 0;
            for (int t = 0; t < 14; ++t) {
                idx_t idx = k - 1 - (idx_t)t;
                unsigned long long abscoeff =
                    coeff[t] >= 0 ? (unsigned long long)coeff[t]
                                  : (unsigned long long)(-coeff[t]);
                unsigned __int128 term =
                    (unsigned __int128)abscoeff *
                    (unsigned __int128)cache[idx];
                if (coeff[t] >= 0)
                    pos += term;
                else
                    neg += term;
            }
            cache[k] = (seq_t)(pos - neg);
        } else {
            /* --- large (≥128‑bit or arbitrary) sequence: direct multiplication --- */
            seq_t pos = 0, neg = 0;
            for (int t = 0; t < 14; ++t) {
                idx_t idx = k - 1 - (idx_t)t;
                seq_t abscoeff = coeff[t] >= 0 ? (seq_t)coeff[t]
                                               : (seq_t)(-coeff[t]);
                seq_t prod = cache[idx] * abscoeff;
                if (coeff[t] >= 0)
                    pos += prod;
                else
                    neg += prod;
            }
            cache[k] = pos - neg;
        }
    }
    max_cached = n;
    return cache[n];
}
