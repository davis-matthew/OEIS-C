/*
 * A269492  Number of length-n 0..6 arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A269492
 *  Number of length‑n 0..6 arrays with no repeated value differing from
 *  the previous repeated value by one.
 *
 *  This implementation uses the 9‑th order linear recurrence
 *
 *      a(n) = 30*a(n-1) - 353*a(n-2) + 1992*a(n-3) - 4976*a(n-4)
 *           + 1872*a(n-5) + 8202*a(n-6) + 1800*a(n-7)
 *           - 210*a(n-8) - 36*a(n-9)
 *
 *  for n ≥ 10, together with the initial values
 *
 *      a(1)=7,  a(2)=49,  a(3)=343,  a(4)=2389,  a(5)=16579,
 *      a(6)=114695, a(7)=791381, a(8)=5448185, a(9)=37435583.
 *
 *  The implementation stores already‑computed terms in a static cache
 *  and grows it as needed; calls may be in any order.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* static cache for already computed values */

static seq_t *cache = NULL;          /* array of a(0), a(1), ... */
static idx_t  cache_size = 0;       /* allocated size of cache */
static idx_t  cache_up = 0;         /* highest n already computed (a(0) unused) */

/* ------------------------------------------------------------ */
/* initial 9 terms (fit in 64 bits) */

static const seq_t init_ten[9] = {
    7ULL,          /* a(1) */
    49ULL,         /* a(2) */
    343ULL,        /* a(3) */
    2389ULL,       /* a(4) */
    16579ULL,      /* a(5) */
    114695ULL,     /* a(6) */
    791381ULL,     /* a(7) */
    5448185ULL,    /* a(8) */
    37435583ULL    /* a(9) */
};

/* ------------------------------------------------------------ */
/* ensure cache has space for indices up to n (inclusive) */

static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    /* initialise new entries to 0 */
    for (idx_t i = cache_size; i <= n; ++i)
        tmp[i] = (seq_t)0;
    cache = tmp;
    cache_size = new_size;
}

/* ------------------------------------------------------------ */
/* main sequence routine */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; a(0) is unused but defined as 0 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..9 */
        cache_size = 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init_ten[i-1];
        cache_up = 9;
    }

    /* enlarge cache if required */
    grow_cache(n);

    /* compute missing values up to n */
    for (idx_t i = cache_up + 1; i <= n; ++i) {
        if (i <= 9) {
            /* initial terms already present; nothing to do */
        } else {
            /* recurrence */
            seq_t pos = cache[i-1] * (seq_t)30;
            pos += cache[i-3] * (seq_t)1992;
            pos += cache[i-5] * (seq_t)1872;
            pos += cache[i-6] * (seq_t)8202;
            pos += cache[i-7] * (seq_t)1800;

            seq_t sub = cache[i-2] * (seq_t)353;
            sub += cache[i-4] * (seq_t)4976;
            sub += cache[i-8] * (seq_t)210;
            sub += cache[i-9] * (seq_t)36;

            cache[i] = pos - sub;   /* subtraction is safe: pos ≥ sub  */
        }
    }
    cache_up = n;
    return cache[n];
}
