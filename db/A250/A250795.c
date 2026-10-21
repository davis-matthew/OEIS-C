/*
 * A250795  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing min(x(i,j),
 *          x(i,j-1)) in the i direction and nondecreasing absolute value of
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) -49*a(n-2) +50*a(n-3) +165*a(n-4) -408*a(n-5)
 *            +a(n-6) +618*a(n-7) -224*a(n-8) -344*a(n-9) +108*a(n-10)
 *            +72*a(n-11) for n >= 12; Initial values: a(1)=530, a(2)=1818,
 *            a(3)=6230, a(4)=21610, a(5)=76110, a(6)=270598, a(7)=968942,
 *            a(8)=3485538, a(9)=12580830, a(10)=45517150, a(11)=164...
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif

#include <stdlib.h>

static const seq_t init_vals[11] = {
    530,
    1818,
    6230,
    21610,
    76110,
    270598,
    968942,
    3485538,
    12580830,
    45517150,
    164970110
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Static state for caching computed terms. */
    static idx_t computed_max = 0;
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    /* Lazy initialization of the initial 11 terms. */
    if (!computed_max) {
        cache.reserve(11);
        for (int i = 0; i < 11; ++i)
            cache.push_back(init_vals[i]);
        computed_max = 11;
    }
    /* Grow the vector if necessary. */
    if (n > cache.size())
        cache.reserve(n);

    /* Compute missing terms. */
    while (computed_max < n) {
        /* Zero‑based index of the new term: a(computed_max+1). */
        idx_t idx = computed_max;

        seq_t pos = 12 * cache[idx - 1]    /* a(n-1) */
                  + 50 * cache[idx - 3]    /* a(n-3) */
                  + 165 * cache[idx - 4]   /* a(n-4) */
                  + 1 * cache[idx - 6]     /* a(n-6) */
                  + 618 * cache[idx - 7]   /* a(n-7) */
                  + 108 * cache[idx - 10]  /* a(n-10) */
                  + 72 * cache[idx - 11];  /* a(n-11) */

        seq_t neg = 49 * cache[idx - 2]    /* a(n-2) */
                  + 408 * cache[idx - 5]   /* a(n-5) */
                  + 224 * cache[idx - 8]   /* a(n-8) */
                  + 344 * cache[idx - 9];  /* a(n-9) */

        cache.push_back(pos - neg);
        ++computed_max;
    }
    return cache[n - 1];
#else
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Lazy initialization of the initial 11 terms. */
    if (!cache) {
        cache_cap = n > 11 ? n : 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                /* allocation failure */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        computed_max = 11;
    }

    /* Grow the buffer if required. */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)
            return (n <= computed_max) ? cache[n - 1] : (seq_t)0;
        cache = tmp;
        cache_cap = n;
    }

    /* Compute missing terms. */
    while (computed_max < n) {
        idx_t idx = computed_max;          /* zero‑based index for a(n) */

        seq_t pos = 12 * cache[idx - 1]    /* a(n-1) */
                  + 50 * cache[idx - 3]    /* a(n-3) */
                  + 165 * cache[idx - 4]   /* a(n-4) */
                  + 1 * cache[idx - 6]     /* a(n-6) */
                  + 618 * cache[idx - 7]   /* a(n-7) */
                  + 108 * cache[idx - 10]  /* a(n-10) */
                  + 72 * cache[idx - 11];  /* a(n-11) */

        seq_t neg = 49 * cache[idx - 2]    /* a(n-2) */
                  + 408 * cache[idx - 5]   /* a(n-5) */
                  + 224 * cache[idx - 8]   /* a(n-8) */
                  + 344 * cache[idx - 9];  /* a(n-9) */

        cache[idx] = pos - neg;
        ++computed_max;
    }
    return cache[n - 1];
#endif
}
