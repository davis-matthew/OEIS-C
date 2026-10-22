/*
 * A351324  Number of tilings of a 7 X 3n rectangle with right trominoes.
 * https://oeis.org/A351324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351324
   Number of tilings of a 7×3n rectangle with right trominoes.
   Recurrence (n ≥ 16):
   a(n) = 22*a(n-1) + 2351*a(n-2) + 40670*a(n-3) + 345038*a(n-4)
          + 3522884*a(n-5) + 28528327*a(n-6) + 145350120*a(n-7)
          + 623982088*a(n-8) + 2110011040*a(n-9) + 1354478796*a(n-10)
          - 9281598624*a(n-11) - 15001687984*a(n-12) - 3456230016*a(n-13)
          + 3194643904*a(n-14) + 1637793792*a(n-15) + 575934464*a(n-16)
   Initial values:
   a(0)=1, a(1)=0, a(2)=520, a(3)=22656, a(4)=1795360,
   a(5)=115363072, a(6)=7876120608, a(7)=527256809600,
   a(8)=35522814546496, a(9)=2388257605782016,
   a(10)=160678147466414272, a(11)=10807663334085120512,
   a(12)=727010169682181839360, a(13)=48903265220016072792320,
   a(14)=3289569236212332037229184, a(15)=221278350342281369716796672
*/

#include <stdlib.h>                 /* for llabs, malloc, realloc */
#include "../utils/seq_from_str.h"   /* for seq_from_str */

static const long long coeff[16] = {
    22LL,          2351LL,      40670LL,
    345038LL,      3522884LL,   28528327LL,
    145350120LL,   623982088LL, 2110011040LL,
    1354478796LL,  -9281598624LL, -15001687984LL,
    -3456230016LL, 3194643904LL, 1637793792LL,
    575934464LL
};

static const char *init_str[16] = {
    "1", "0", "520", "22656", "1795360", "115363072",
    "7876120608", "527256809600", "35522814546496",
    "2388257605782016", "160678147466414272", "10807663334085120512",
    "727010169682181839360", "48903265220016072792320",
    "3289569236212332037229184", "221278350342281369716796672"
};

static seq_t init_vals[16];
static int init_done = 0;

static seq_t *cache = NULL;
static idx_t cache_len = 0;     /* number of values stored (>=0) */
static size_t cache_cap = 0;    /* allocated capacity */

static void init_cache(void)
{
    if (init_done) return;
    for (int i = 0; i < 16; ++i)
        init_vals[i] = seq_from_str(init_str[i]);
    init_done = 1;
}

static void ensure_cache(idx_t n)
{
    if (!init_done) init_cache();

    /* allocate minimal cache if not yet done */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (uint64_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cache_len = 16;
    }

    /* grow capacity if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 16; ++k) {
                seq_t term = cache[i - 1 - k] * (seq_t)llabs(coeff[k]);
                if (coeff[k] >= 0)
                    pos += term;
                else
                    neg += term;
            }
            cache[i] = pos - neg;      /* pos ≥ neg by the recurrence */
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 16) {
        if (!init_done) init_cache();
        return init_vals[(int)n];
    }
    ensure_cache(n);
    return cache[n];
}
