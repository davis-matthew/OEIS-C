/*
 * A269474  Column 2 of A266240.
 * https://oeis.org/A269474
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   * (n‑4)*(n‑3)*(n‑2)*(2100*n^3 – 22577*n^2 + 77702*n –
 *            86760)*a(n) * = 48*(n‑4)*(37800*n^5 – 463086*n^4 + 2015601*n^3 –
 *            3767956*n^2 * + 2822996*n – 645120)*a(n‑2) * – 20736*(n‑2)*(3*n
 *            – 10)*(3*n – 8)*(2100*n^3 – 9977*n^2 + 12594*n – 4864)*a(n‑4) *
 *            * Initial values (offset 3): * a(3)=105, a(4)=8112...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if !defined(__cplusplus)
/* C implementation ---------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 big_t;      /* 128‑bit intermediate for 64‑bit seq_t   */
#else
typedef seq_t big_t;                 /* fallback – used for arbitrary‑precision seq_t   */
#endif

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cur_max   = 0;

    /* initialise cache if empty */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        if (cache_cap > 3) cache[3] = (seq_t)105;
        if (cache_cap > 4) cache[4] = (seq_t)8112;
        if (cache_cap > 5) cache[5] = (seq_t)396792;
        if (cache_cap > 6) cache[6] = (seq_t)15663360;
        cur_max   = 6;
    }
    /* grow cache if needed */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
        if (cache_cap > 3 && cache[3] == 0) cache[3] = (seq_t)105;
        if (cache_cap > 4 && cache[4] == 0) cache[4] = (seq_t)8112;
        if (cache_cap > 5 && cache[5] == 0) cache[5] = (seq_t)396792;
        if (cache_cap > 6 && cache[6] == 0) cache[6] = (seq_t)15663360;
    }

    /* return cached value if already computed */
    if (n <= cur_max) return cache[(size_t)n];

    /* compute missing terms in ascending order */
    for (size_t i = cur_max + 1; i <= (size_t)n; ++i) {
        if (i <= 6) continue;                 /* all base cases initialised */

        idx_t ii = (idx_t)i;
        big_t n_big = (big_t)ii;
        big_t n2 = n_big * n_big;
        big_t n3 = n2 * n_big;
        big_t n4 = n2 * n2;
        big_t n5 = n3 * n2;

        big_t poly1 = (big_t)2100 * n3 - (big_t)22577 * n2 + (big_t)77702 * n_big - (big_t)86760;
        big_t poly2 = (big_t)37800 * n5 - (big_t)463086 * n4 + (big_t)2015601 * n3
                    - (big_t)3767956 * n2 + (big_t)2822996 * n_big - (big_t)645120;
        big_t poly3 = (big_t)2100 * n3 - (big_t)9977 * n2 + (big_t)12594 * n_big - (big_t)4864;

        big_t nm4 = (big_t)(ii - 4);
        big_t nm3 = (big_t)(ii - 3);
        big_t nm2 = (big_t)(ii - 2);

        big_t denom = nm4 * nm3 * nm2 * poly1;
        big_t term1  = (big_t)48 * nm4 * poly2 * cache[i - 2];
        big_t term2  = (big_t)20736 * nm2 * (big_t)(3 * ii - 10)
                      * (big_t)(3 * ii - 8) * poly3 * cache[i - 4];
        big_t value  = (term1 - term2) / denom;

        cache[i] = (seq_t)value;
    }

    cur_max = (idx_t)n;
    return cache[(size_t)n];
}
#else
/* C++ implementation --------------------------------------------------------- */
#include <cstdint>
#include <type_traits>

/* if seq_t is a 64‑bit integral type, use unsigned __int128 for intermediates;
   otherwise use seq_t (handles arbitrary‑precision types). */
using big_t = typename std::conditional<(sizeof(seq_t) <= 8), unsigned __int128, seq_t>::type;

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    static seq_t *cache  = nullptr;
    static size_t cache_cap = 0;
    static idx_t cur_max   = 0;

    /* initialise cache if empty */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)std::malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        if (cache_cap > 3) cache[3] = (seq_t)105;
        if (cache_cap > 4) cache[4] = (seq_t)8112;
        if (cache_cap > 5) cache[5] = (seq_t)396792;
        if (cache_cap > 6) cache[6] = (seq_t)15663360;
        cur_max   = 6;
    }
    /* grow cache if needed */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)std::realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
        if (cache_cap > 3 && cache[3] == 0) cache[3] = (seq_t)105;
        if (cache_cap > 4 && cache[4] == 0) cache[4] = (seq_t)8112;
        if (cache_cap > 5 && cache[5] == 0) cache[5] = (seq_t)396792;
        if (cache_cap > 6 && cache[6] == 0) cache[6] = (seq_t)15663360;
    }

    /* return cached value if already computed */
    if (n <= cur_max) return cache[(size_t)n];

    /* compute missing terms in ascending order */
    for (size_t i = cur_max + 1; i <= (size_t)n; ++i) {
        if (i <= 6) continue;                 // base values already initialised

        idx_t ii = (idx_t)i;
        big_t n_big = (big_t)ii;
        big_t n2 = n_big * n_big;
        big_t n3 = n2 * n_big;
        big_t n4 = n2 * n2;
        big_t n5 = n3 * n2;

        big_t poly1 = (big_t)2100 * n3 - (big_t)22577 * n2 + (big_t)77702 * n_big - (big_t)86760;
        big_t poly2 = (big_t)37800 * n5 - (big_t)463086 * n4 + (big_t)2015601 * n3
                    - (big_t)3767956 * n2 + (big_t)2822996 * n_big - (big_t)645120;
        big_t poly3 = (big_t)2100 * n3 - (big_t)9977 * n2 + (big_t)12594 * n_big - (big_t)4864;

        big_t nm4 = (big_t)(ii - 4);
        big_t nm3 = (big_t)(ii - 3);
        big_t nm2 = (big_t)(ii - 2);

        big_t denom = nm4 * nm3 * nm2 * poly1;
        big_t term1  = (big_t)48 * nm4 * poly2 * cache[i - 2];
        big_t term2  = (big_t)20736 * nm2 * (big_t)(3 * ii - 10)
                      * (big_t)(3 * ii - 8) * poly3 * cache[i - 4];
        big_t value  = (term1 - term2) / denom;

        cache[i] = (seq_t)value;
    }

    cur_max = (idx_t)n;
    return cache[(size_t)n];
}
#endif
