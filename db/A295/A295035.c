/*
 * A295035  Number of nX3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 2 1s.
 * https://oeis.org/A295035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the empirical recurrence * a(1) … a(15) given
 *            below, a(n) = 4*a(n-1) + 2*a(n-2) +17*a(n-3) +18*a(n-4) *
 *            +53*a(n-5) -23*a(n-6) -38*a(n-7) +52*a(n-8) * -19*a(n-9)
 *            -14*a(n-10) +16*a(n-11) -11*a(n-12) * + a(n-14) - a(n-15) for n
 *            ≥ 16
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Assumes seq_t and idx_t are typedef'd by the build system. */

static seq_t *cache = NULL;     /* 1‑based array: cache[n] = a(n) */
static size_t cap   = 0;        /* allocated size (>= n+1) */
static idx_t computed = 0;      /* largest n already computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cap) return;
    size_t new_cap = (size_t)n + 1;          /* 1‑based indexing */
    if (new_cap < 16) new_cap = 16;          /* at least space for 1…15 */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise the cache with the first 15 terms. */
static void init_base_values(void)
{
    static const seq_t base_init[16] = {
        0ULL,                                   /* unused 0‑th element */
        (seq_t)4ULL,                            /* a(1) */
        (seq_t)29ULL,                           /* a(2) */
        (seq_t)140ULL,                          /* a(3) */
        (seq_t)741ULL,                          /* a(4) */
        (seq_t)3853ULL,                         /* a(5) */
        (seq_t)19956ULL,                        /* a(6) */
        (seq_t)104096ULL,                       /* a(7) */
        (seq_t)541775ULL,                       /* a(8) */
        (seq_t)2819027ULL,                      /* a(9) */
        (seq_t)14671776ULL,                     /* a(10) */
        (seq_t)76356631ULL,                     /* a(11) */
        (seq_t)397392690ULL,                    /* a(12) */
        (seq_t)2068192977ULL,                   /* a(13) */
        (seq_t)10763659993ULL,                  /* a(14) */
        (seq_t)56018290276ULL                   /* a(15) */
    };
    ensure_cache_size(15);
    for (idx_t i = 1; i <= 15; ++i)
        cache[i] = base_init[i];
    computed = 15;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n = 0, defensive */
    ensure_cache_size(n);

    if (computed < 15) init_base_values();   /* first time we need data */

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 15) continue;        /* base values already set */
        /* recurrence for i ≥ 16 */
        seq_t val =
            (seq_t)4 * cache[i-1] +
            (seq_t)2 * cache[i-2] +
            (seq_t)17 * cache[i-3] +
            (seq_t)18 * cache[i-4] +
            (seq_t)53 * cache[i-5] -
            (seq_t)23 * cache[i-6] -
            (seq_t)38 * cache[i-7] +
            (seq_t)52 * cache[i-8] -
            (seq_t)19 * cache[i-9] -
            (seq_t)14 * cache[i-10] +
            (seq_t)16 * cache[i-11] -
            (seq_t)11 * cache[i-12] +
            cache[i-14] -
            cache[i-15];
        cache[i] = val;
    }
    if (computed < n) computed = n;
    return cache[n];
}
