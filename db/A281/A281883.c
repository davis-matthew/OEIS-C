/*
 * A281883  Number of n X 3 0..1 arrays with no element equal to more than
 *          four of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) – 17*a(n-2) – 28*a(n-3) – 122*a(n-4) +
 *            132*a(n-5) + 368*a(n-6) + 570*a(n-7) + 171*a(n-8) – 746*a(n-9) –
 *            979*a(n-10) – 2038*a(n-11) – 1625*a(n-12) – 840*a(n-13) –
 *            784*a(n-14) (n > 17) with a(1)=0, a(2)=0, a(3)=68, a(4)=638,
 *            a(5)=4832, a(6)=35002, a(7)=241209, a(8)=1612568, a(9)=10...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard facilities for memory allocation */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc / realloc */

/* Initial values up to n = 17 (1‑based indices) */
static const seq_t init_vals[17] = {
    (seq_t)0,            /* a(1) */
    (seq_t)0,            /* a(2) */
    (seq_t)68,           /* a(3) */
    (seq_t)638,          /* a(4) */
    (seq_t)4832,         /* a(5) */
    (seq_t)35002,        /* a(6) */
    (seq_t)241209,       /* a(7) */
    (seq_t)1612568,      /* a(8) */
    (seq_t)10566034,     /* a(9) */
    (seq_t)68136376,     /* a(10) */
    (seq_t)433938423,    /* a(11) */
    (seq_t)2736019588,   /* a(12) */
    (seq_t)17108229189,  /* a(13) */
    (seq_t)106234294380, /* a(14) */
    (seq_t)655764472080, /* a(15) */
    (seq_t)4027264205666,/* a(16) */
    (seq_t)24622851384305/* a(17) */
};

/* Cache for computed terms.  1‑based indexing: cache[k-1] = a(k). */
static seq_t *cache      = NULL;  /* pointer to allocated array */
static size_t cache_cap  = 0;     /* current allocated length */
static size_t cache_len  = 0;     /* number of elements that have been filled */

/* Allocate or enlarge the cache to hold at least 'new_cap' elements. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_ptr = NULL;
    if (cache_cap == 0) {
        new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n) for any order of calls.  Cache previous results. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for OEIS, but defined safely */
        return (seq_t)0;

    /* Initialise the cache on first call */
    if (cache == NULL) {
        ensure_cache_capacity(17);
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init_vals[i];
        cache_len = 17;
    }

    /* Grow the cache if necessary */
    size_t target = (size_t)n;
    if (target > cache_cap)
        ensure_cache_capacity(target);

    /* Compute any missing terms up to n */
    if (target > cache_len) {
        for (size_t i = cache_len + 1; i <= target; ++i) {
            seq_t val = 0;
            val += (seq_t)10   * cache[i - 2];  /* a(i-1) */
            val -= (seq_t)17   * cache[i - 3];  /* a(i-2) */
            val -= (seq_t)28   * cache[i - 4];  /* a(i-3) */
            val -= (seq_t)122  * cache[i - 5];  /* a(i-4) */
            val += (seq_t)132  * cache[i - 6];  /* a(i-5) */
            val += (seq_t)368  * cache[i - 7];  /* a(i-6) */
            val += (seq_t)570  * cache[i - 8];  /* a(i-7) */
            val += (seq_t)171  * cache[i - 9];  /* a(i-8) */
            val -= (seq_t)746  * cache[i -10];  /* a(i-9) */
            val -= (seq_t)979  * cache[i -11];  /* a(i-10) */
            val -= (seq_t)2038 * cache[i -12];  /* a(i-11) */
            val -= (seq_t)1625 * cache[i -13];  /* a(i-12) */
            val -= (seq_t)840  * cache[i -14];  /* a(i-13) */
            val -= (seq_t)784  * cache[i -15];  /* a(i-14) */
            cache[i - 1] = val;                 /* store a(i) */
        }
        cache_len = target;
    }

    return cache[target - 1];   /* a(n) */
}
