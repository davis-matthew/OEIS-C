/*
 * A260288  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00001001.
 * https://oeis.org/A260288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 6*a(n-3) + 4*a(n-4) - 5*a(n-5) - 12*a(n-6) +
 *            8*a(n-7) + a(n-8) - 11*a(n-9) + 3*a(n-10) Initial values:
 *            a(1)=104, a(2)=201, a(3)=544, a(4)=1145, a(5)=1524, a(6)=3591,
 *            a(7)=8550, a(8)=13055, a(9)=24762, a(10)=60761, a(11)=108910,
 *            a(12)=183673
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 59, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static const seq_t base_vals[12] = {
    104, 201, 544, 1145, 1524, 3591,
    8550, 13055, 24762, 60761, 108910, 183673
};

#ifdef __cplusplus
/* C++ version – use a vector so constructors are called. */
#include <vector>
static std::vector<seq_t> cache;

/* Initialise the cache with the 12 known terms. */
static void init_cache_cpp(void)
{
    if (cache.empty()) {
        cache.insert(cache.end(), base_vals, base_vals + 12);   /* a(1)..a(12) */
    }
}

/* Compute a(n) for n >= 1 (C++ with arbitrary‑precision seq_t). */
seq_t a(idx_t n)
{
    assert(n >= 1);
    init_cache_cpp();

    if (n <= 12) {
        /* cache is 0‑based and already holding a(1)..a(12) */
        return cache[n - 1];
    }

    /* Grow the vector as needed. */
    while (cache.size() < (size_t)n) {
        idx_t k = (idx_t)cache.size() + 1;          /* current n value */

        seq_t term = cache[k - 3];                  /* a(k-2)   */
        term += cache[k - 4] * 6;                  /* 6*a(k-3) */
        term += cache[k - 5] * 4;                  /* 4*a(k-4) */
        term -= cache[k - 6] * 5;                  /* 5*a(k-5) */
        term -= cache[k - 7] * 12;                 /* 12*a(k-6)*/
        term += cache[k - 8] * 8;                  /* 8*a(k-7) */
        term += cache[k - 9];                      /* a(k-8)   */
        term -= cache[k - 10] * 11;                /* 11*a(k-9)*/
        term += cache[k - 11] * 3;                 /* 3*a(k-10)*/

        cache.emplace_back(term);
    }

    return cache[n - 1];
}
#else
/* C version – use malloc/realloc (seq_t is a plain integer type). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */

/* Initialise the cache with the 12 known terms. */
static void init_cache_c(void)
{
    if (cache_cap == 0) {
        cache_cap = 64;      /* initial capacity, will grow as needed */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* OOM */
        for (int i = 0; i < 12; ++i) {
            cache[i] = base_vals[i];
        }
    }
}

/* Ensure the cache can hold at least n elements (indices 0..n-1). */
static void ensure_cache_c(idx_t n)
{
    if ((size_t)n <= cache_cap) return;

    size_t new_cap = cache_cap * 2;
    while ((size_t)n > new_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* OOM */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1 (C with plain integer seq_t). */
seq_t a(idx_t n)
{
    assert(n >= 1);
    init_cache_c();

    if (n <= 12) {
        return cache[n - 1];
    }

    /* Grow cache so that it can store a(n) (index n-1). */
    ensure_cache_c(n);

    /* Compute missing terms. */
    for (idx_t k = 13; (size_t)k <= n; ++k) {
        seq_t term = cache[k - 3];                  /* a(k-2)   */
        term += cache[k - 4] * 6;                  /* 6*a(k-3) */
        term += cache[k - 5] * 4;                  /* 4*a(k-4) */
        term -= cache[k - 6] * 5;                  /* 5*a(k-5) */
        term -= cache[k - 7] * 12;                 /* 12*a(k-6)*/
        term += cache[k - 8] * 8;                  /* 8*a(k-7) */
        term += cache[k - 9];                      /* a(k-8)   */
        term -= cache[k - 10] * 11;                /* 11*a(k-9)*/
        term += cache[k - 11] * 3;                 /* 3*a(k-10)*/
        cache[k - 1] = term;                       /* store a(k) */
    }

    return cache[n - 1];
}
#endif
