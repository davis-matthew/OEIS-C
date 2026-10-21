/*
 * A245114  G.f. A(x) satisfies A(x)^3 = 1 + 9*x*A(x)^5.
 * https://oeis.org/A245114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   2*a(n+3)*(n+3)*(n+2)*(n+1)*(2*n+7) =
 *            135*a(n)*(5*n+1)*(5*n+4)*(5*n+7)*(5*n+13) (a(0)=1, a(1)=3,
 *            a(2)=36)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine below computes a(n) for any n≥0.
   It uses memoisation so the order of calls is irrelevant.
   For plain‑C builds the calculation uses unsigned __int128
   temporaries to avoid overflow while still producing the
   exact result for the 64‑bit data type.
   For C++ builds (arbitrary‑precision seq_t) the calculation
   is performed with normal seq_t arithmetic. */

#ifdef __cplusplus
#include <vector>
#include <cstddef>

static std::vector<seq_t> cache;

static seq_t compute_next(idx_t m, seq_t a_m)
{
    seq_t num = a_m;
    num *= 135;
    num *= (seq_t)(5*m + 1);
    num *= (seq_t)(5*m + 4);
    num *= (seq_t)(5*m + 7);
    num *= (seq_t)(5*m + 13);

    seq_t den = 1;
    den *= 2;
    den *= (seq_t)(m + 3);
    den *= (seq_t)(m + 2);
    den *= (seq_t)(m + 1);
    den *= (seq_t)(2*m + 7);

    return num / den;
}

seq_t a(idx_t n)
{
    if (cache.empty()) {          /* initialise base terms */
        cache.reserve(3);
        cache.push_back((seq_t)1);  /* a(0) */
        cache.push_back((seq_t)3);  /* a(1) */
        cache.push_back((seq_t)36); /* a(2) */
    }

    if (n < cache.size())
        return cache[(size_t)n];

    for (idx_t i = (idx_t)cache.size(); i <= n; ++i)
    {
        idx_t m = i - 3;           /* compute a(i) from a(i-3) */
        seq_t next = compute_next(m, cache[(size_t)m]);
        cache.push_back(next);
    }
    return cache[(size_t)n];
}

#else   /* Plain C build */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static seq_t compute_next(idx_t m, seq_t a_m)
{
    /* Unsigned 128‑bit temporaries are available on modern compilers. */
    unsigned __int128 num = (unsigned __int128)a_m;
    num *= 135;
    num *= (unsigned __int128)(5*m + 1);
    num *= (unsigned __int128)(5*m + 4);
    num *= (unsigned __int128)(5*m + 7);
    num *= (unsigned __int128)(5*m + 13);

    unsigned __int128 den = 2;
    den *= (unsigned __int128)(m + 3);
    den *= (unsigned __int128)(m + 2);
    den *= (unsigned __int128)(m + 1);
    den *= (unsigned __int128)(2*m + 7);

    return (seq_t)(num / den);
}

seq_t a(idx_t n)
{
    if (cache == NULL) {          /* initialise base terms */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)3;       /* a(1) */
        cache[2] = (seq_t)36;      /* a(2) */
        cache_size = 3;
    }

    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to index n. */
    if (cache_size <= n) {
        seq_t *tmp = (seq_t *)realloc(cache,
                         (size_t)(n + 1) * sizeof(seq_t));
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t m = i - 3;
            cache[i] = compute_next(m, cache[m]);
        }
        cache_size = n + 1;
    }
    return cache[n];
}

#endif
