/*
 * A269181  Number of nX3 0..3 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling three exactly once.
 * https://oeis.org/A269181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 131*a(n-2) + 88*a(n-3) + 221*a(n-4) -
 *            154*a(n-5) - 101*a(n-6) + 44*a(n-7) - 4*a(n-8) for n > 9 a(1) =
 *            24, a(2) = 384, a(3) = 5888, a(4) = 80112, a(5) = 1031344, a(6)
 *            = 12791896, a(7) = 154606864, a(8) = 1833130768, a(9) =
 *            21416076480 (These initial values are hard‑wired into th...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* cache of computed terms, zero‑based */
static size_t  siz    = 0;      /* number of cached terms          */
static size_t  cap    = 0;      /* capacity of cache array         */

/* initialise the cache with the first nine terms */
static void init_cache(void)
{
    if (cache != NULL) return;

    cap   = 16;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    cache[0] = (seq_t)24ULL;          /* a(1)  */
    cache[1] = (seq_t)384ULL;         /* a(2)  */
    cache[2] = (seq_t)5888ULL;        /* a(3)  */
    cache[3] = (seq_t)80112ULL;       /* a(4)  */
    cache[4] = (seq_t)1031344ULL;     /* a(5)  */
    cache[5] = (seq_t)12791896ULL;    /* a(6)  */
    cache[6] = (seq_t)154606864ULL;   /* a(7)  */
    cache[7] = (seq_t)1833130768ULL;  /* a(8)  */
    cache[8] = (seq_t)21416076480ULL; /* a(9)  */

    siz = 9;
}

/* grow the cache array to hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cap   = new_cap;
}

#ifdef __cplusplus
/* C++ version – works for arbitrary‑precision seq_t types. */
static inline seq_t compute_term_cpp(size_t i)
{
    seq_t pos = 0, neg = 0;

    pos += cache[i-1] * 22ULL;
    pos += cache[i-3] * 88ULL;
    pos += cache[i-4] * 221ULL;
    pos += cache[i-7] * 44ULL;

    neg += cache[i-2] * 131ULL;
    neg += cache[i-5] * 154ULL;
    neg += cache[i-6] * 101ULL;
    neg += cache[i-8] * 4ULL;

    cache[i] = pos - neg;
    return cache[i];
}
#else
/* C version – for built‑in unsigned types (uint64_t etc.). */
static void compute_term_c(size_t i)
{
    __uint128_t pos = 0, neg = 0;

    pos += (__uint128_t)cache[i-1] * 22ULL;
    pos += (__uint128_t)cache[i-3] * 88ULL;
    pos += (__uint128_t)cache[i-4] * 221ULL;
    pos += (__uint128_t)cache[i-7] * 44ULL;

    neg += (__uint128_t)cache[i-2] * 131ULL;
    neg += (__uint128_t)cache[i-5] * 154ULL;
    neg += (__uint128_t)cache[i-6] * 101ULL;
    neg += (__uint128_t)cache[i-8] * 4ULL;

    cache[i] = (seq_t)(pos - neg);
}
#endif

/* public function required by the task */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n = 0 */

    init_cache();

    size_t idx = (size_t)n - 1;         /* zero‑based index */

    if (idx < siz)                       /* already cached */
        return cache[idx];

    grow_cache(idx + 1);

    /* compute terms up to idx */
    for (size_t i = siz; i <= idx; ++i)
    {
#ifdef __cplusplus
        compute_term_cpp(i);
#else
        compute_term_c(i);
#endif
    }

    siz = idx + 1;
    return cache[idx];
}
