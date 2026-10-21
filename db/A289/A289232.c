/*
 * A289232  Number of nonequivalent ways to select 5 disjoint point triples
 *          from an n X n X n triangular point grid, each point triple forming
 *          a 2 X 2 X 2 triangle.
 * https://oeis.org/A289232
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 -10*n^9 -85*n^8 +1160*n^7 +1345*n^6 -49084*n^5 *
 *            +61035*n^4 +897210*n^3 -2205196*n^2 -5725656*n +18174960)/720 *
 *            + IF(MOD(n, 2) = 1, -2*n^2 +13*n -11)/4. * This implementation
 *            uses the linear recurrence derived from the * generating
 *            function: * a(n) = - (d1*a(n-1) + d2*a(n-2) + ... +...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 19, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_terms[14] = {
    (seq_t)0ULL,               /* a(5) */
    (seq_t)77ULL,              /* a(6) */
    (seq_t)2569ULL,            /* a(7) */
    (seq_t)31951ULL,           /* a(8) */
    (seq_t)223346ULL,          /* a(9) */
    (seq_t)1089665ULL,         /* a(10) */
    (seq_t)4161705ULL,         /* a(11) */
    (seq_t)13314461ULL,        /* a(12) */
    (seq_t)37246668ULL,        /* a(13) */
    (seq_t)93781829ULL,        /* a(14) */
    (seq_t)216901737ULL,       /* a(15) */
    (seq_t)467727523ULL,       /* a(16) */
    (seq_t)951014654ULL,       /* a(17) */
    (seq_t)1839155785ULL        /* a(18) */
};

static const int coeff[15] = {
    1,      /* d0, not used in recurrence */
    -8,     /* d1  */
    25,     /* d2  */
    -32,    /* d3  */
    -11,    /* d4  */
    88,     /* d5  */
    -99,    /* d6  */
    0,      /* d7  */
    99,     /* d8  */
    -88,    /* d9  */
    11,     /* d10 */
    32,     /* d11 */
    -25,    /* d12 */
    8,      /* d13 */
    -1      /* d14 */
};

static seq_t *cache = NULL;       /* cache[n] holds a(n) for n >= 0 */
static idx_t cache_size = 0;      /* allocated size of cache array    */
static idx_t computed_upto = 4;   /* highest n that has been computed */

static void ensure_capacity(idx_t n) {
    if (cache_size > n) return;
    idx_t new_size = cache_size ? cache_size * 2 : 64;
    while (new_size <= n) new_size <<= 1;
    seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;
}

/* Initialize cache with values for n = 5 … 18 */
static void init_cache(void) {
    if (cache) return; /* already initialized */
    ensure_capacity(18);
    for (idx_t i = 5; i <= 18; ++i) {
        cache[i] = init_terms[i - 5];
    }
    computed_upto = 18;
}

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;

    init_cache();
    ensure_capacity(n);

    if (n > computed_upto) {
        /* Compute up to n using the recurrence */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t val = (seq_t)0;
            /* a(i) = - (d1*a(i-1) + ... + d14*a(i-14)) */
            for (int k = 1; k <= 14; ++k) {
                val -= (seq_t)coeff[k] * cache[i - k];
            }
            cache[i] = val;
        }
        computed_upto = n;
    }

    return cache[n];
}
