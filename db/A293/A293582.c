/*
 * A293582  Number of compositions of n where each part i is marked with a
 *          word of length i over a quinary alphabet whose letters appear in
 *          alphabetical order and all five letters occur at least once in the
 *          composition.
 * https://oeis.org/A293582
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 380*a(n-2) + 2690*a(n-3) - 11944*a(n-4) + *
 *            35618*a(n-5) - 74912*a(n-6) + 115104*a(n-7) - 132120*a(n-8) + *
 *            114500*a(n-9) - 74888*a(n-10) + 36504*a(n-11) - 12888*a(n-12) +
 *            * 3120*a(n-13) - 464*a(n-14) + 32*a(n-15). Initial values: *
 *            a(5)=541, a(6)=13060, a(7)=195020, a(8)=23279...
 * Range:     u8 none, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_seq_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                       /* abs() */
#include "../utils/alloc_seq_array.h"     /* alloc_seq_array() */
#include "../utils/seq_from_str.h"        /* seq_from_str() */

static const int coeff[15] = {30, -380, 2690, -11944, 35618,
                              -74912, 115104, -132120, 114500,
                              -74888, 36504, -12888, 3120,
                              -464, 32};

static const char *init_str[15] = {
    "541", "13060", "195020", "2327960", "24418640",
    "235804122", "2152586500", "18883155160", "160908360260",
    "1341800118020", "11007289244964", "89168468504160",
    "715330888641680", "5694960569676240", "45067846839572000"
};

static seq_t init_val[15];
static bool init_done = false;

/* Convert the string constants to seq_t once */
static void init_constants(void) {
    for (int i = 0; i < 15; ++i)
        init_val[i] = seq_from_str(init_str[i]);
    init_done = true;
}

/* Cache of all computed terms up to the largest index requested */
static seq_t *cache = NULL;           /* array accessed by index n */
static size_t cache_cap = 0;          /* allocated size of cache */
static idx_t cache_max = 4;           /* largest n already computed */

/* Ensure the cache contains values up to n */
static void ensure_cache(idx_t n) {
    if (n <= cache_max)
        return;

    if (!init_done)
        init_constants();

    /* resize cache if necessary */
    if (cache == NULL || cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (cache != NULL) {
            /* copy old values */
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* fill missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 19) {                     /* first 15 terms */
            cache[i] = init_val[i - 5];
        } else {                           /* recurrence */
            seq_t pos = 0, neg = 0;
            for (int j = 0; j < 15; ++j) {
                seq_t term = cache[i - 1 - j] *
                             (seq_t)(abs(coeff[j]));
                if (coeff[j] >= 0)
                    pos += term;
                else
                    neg += term;
            }
            cache[i] = pos - neg;
        }
    }

    cache_max = n;
}

seq_t a(idx_t n) {
    if (n < 5)
        return 0;
    ensure_cache(n);
    return cache[n];
}
