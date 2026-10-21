/*
 * A263946  Positive integers n such that (n+52)^3 - n^3 is a square.
 * https://oeis.org/A263946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A263946: a(1)=26, a(2)=2626, a(3)=132522,
   recurrence: a(n)=51*a(n-1)-51*a(n-2)+a(n-3) for n>3
*/
#include <stddef.h>
#include <stdint.h>
#if defined(__cplusplus)
#  include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    /*--- C++ path -----------------------------------------------------*/
    #if defined(__cplusplus)
    /* use a static std::vector for arbitrary‑precision seq_t */
    static std::vector<seq_t> cache;   /* cache[i] holds a(i+1) */

    if (n <= cache.size())
        return cache[(size_t)n - 1];

    /* initialise missing base values */
    if (cache.size() < 1) cache.push_back((seq_t)26);     /* a(1) */
    if (cache.size() < 2) cache.push_back((seq_t)2626);   /* a(2) */
    if (cache.size() < 3) cache.push_back((seq_t)132522); /* a(3) */

    /* compute subsequent terms */
    while (cache.size() < (size_t)n) {
        size_t i = cache.size();
        /* i corresponds to a(i+1) */
        seq_t term = (seq_t)51 * cache[i-1]
                   - (seq_t)51 * cache[i-2]
                   +           cache[i-3];
        cache.push_back(term);
    }
    return cache[(size_t)n - 1];
    #else
    /*--- C path -------------------------------------------------------*/
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* number of stored terms */

    size_t idx = (size_t)n - 1; /* array index for a(n) */
    if (idx < cap) 
        return cache[idx];

    /* grow the cache to at least idx+1 elements */
    size_t new_cap = idx + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failed */
    cache = tmp;

    /* initialise base values that are missing */
    if (cap == 0) cache[0] = (seq_t)26;
    if (cap <= 1) cache[1] = (seq_t)2626;
    if (cap <= 2) cache[2] = (seq_t)132522;

    /* compute remaining terms */
    for (size_t i = cap; i < new_cap; ++i) {
        if (i < 3) continue;                 /* base values set */
        cache[i] = (seq_t)51 * cache[i-1]
                 - (seq_t)51 * cache[i-2]
                 +          cache[i-3];
    }

    cap = new_cap;
    return cache[idx];
    #endif
}
