/*
 * A289707  Number of 6-cycles in the n-triangular honeycomb queen graph.
 * https://oeis.org/A289707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-10*a(n-3)+3*a(n-4)+13*a(n-5)-3*a(n-6)
 *            -12*a(n-7)-3*a(n-8)+13*a(n-9)+3*a(n-10)-10*a(n-11)+a(n-12)
 *            +3*a(n-13)-a(n-14) with a(1)=0, a(2)=0, a(3)=16, a(4)=911,
 *            a(5)=8013, a(6)=38130, a(7)=129932, a(8)=358272, a(9)=851710,
 *            a(10)=1815124, a(11)=3554910, a(12)=6510729, a(13)=11289...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 31, u64 n <= 895, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed values */
static size_t cap   = 0;             /* current capacity (number of elements) */
static idx_t  last  = 0;             /* largest n that has been computed */

/* initial values a(1)...a(14) */
static const seq_t init[15] = {
    0ULL,          /* index 0 unused */
    0ULL,          /* a(1) */
    0ULL,          /* a(2) */
    16ULL,         /* a(3) */
    911ULL,        /* a(4) */
    8013ULL,       /* a(5) */
    38130ULL,      /* a(6) */
    129932ULL,     /* a(7) */
    358272ULL,     /* a(8) */
    851710ULL,     /* a(9) */
    1815124ULL,    /* a(10) */
    3554910ULL,    /* a(11) */
    6510729ULL,    /* a(12) */
    11289019ULL,   /* a(13) */
    18704640ULL    /* a(14) */
};

/* Ensure the cache can hold indices up to n (1‑based indices). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;            /* include unused index 0 */
    if (cap < needed) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out‑of‑range, not used */

    if (n <= 14) return init[(size_t)n];

    ensure_capacity(n);

    /* initialise the first 14 entries once */
    if (last < 14) {
        for (size_t i = 1; i <= 14 && i <= (size_t)n; ++i) {
            cache[i] = init[i];
        }
        last = 14;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] =
            3 * cache[i-1] + cache[i-2] - 10 * cache[i-3] + 3 * cache[i-4]
          + 13 * cache[i-5] - 3 * cache[i-6] - 12 * cache[i-7]
          - 3 * cache[i-8] + 13 * cache[i-9]
          + 3 * cache[i-10] - 10 * cache[i-11] + cache[i-12]
          + 3 * cache[i-13] - cache[i-14];
    }
    last = n;
    return cache[n];
}
