/*
 * A273898  Sum of the abscissae of the first descents of all bargraphs of
 *          semiperimeter n (n>=2).
 * https://oeis.org/A273898
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (n+3)*a(n)+2*(-3*n-4)*a(n-1)+2*(5*n-2)*a(n-2)
 *            +4*(-n+2)*a(n-3)+(n-3)*a(n-4)+2*(-n+5)*a(n-5)=0 with a(2)=1,
 *            a(3)=3, a(4)=9, a(5)=27, a(6)=81. a(n) = [ 2*(3*n+4)*a(n-1) -
 *            2*(5*n-2)*a(n-2) + 4*(n-2)*a(n-3) - (n-3)*a(n-4) +
 *            2*(n-5)*a(n-5) ] / (n+3) for n >= 7. The sequence is cached
 *            to...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* below offset: defined as 0 */

    /* C++ implementation (supports arbitrary‑precision seq_t) */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static bool initialized = false;

    if (!initialized) {
        /* indices 0..6, set known values at 2..6, the rest default to 0 */
        cache.resize(7);
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)9;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)81;
        initialized = true;
    }

    /* Ensure the vector can store up to index n (no shrink) */
    size_t cur_size = cache.size();
    if (cur_size <= n) {
        cache.resize(n + 1);
    }

    /* Compute new values if needed, starting from the first uncomputed index */
    idx_t start = (cur_size <= 6) ? 7 : (idx_t)cur_size;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term1 = cache[i-1] * (seq_t)(2 * (3*i + 4));
        seq_t term2 = cache[i-2] * (seq_t)(10*i - 4); /* coefficient 2*(5*i-2) */
        seq_t term3 = cache[i-3] * (seq_t)(4 * (i-2));
        seq_t term4 = cache[i-4] * (seq_t)(i - 3);
        seq_t term5 = cache[i-5] * (seq_t)(2 * (i - 5));
        seq_t num   = term1 - term2 + term3 - term4 + term5;
        cache[i]    = num / (seq_t)(i + 3);
    }

    return cache[(size_t)n];

#else   /* C implementation – works when seq_t is an unsigned integral type */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t cached_upto = 0;

    /* Ensure we have space for index n */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        size_t new_cap = capacity ? capacity << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        if (new_cap < 7) new_cap = 7;          /* need indices up to 6 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise known values on first use */
    if (cached_upto < 6) {
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)9;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)81;
        cached_upto = 6;
    }

    /* Compute new values if needed */
    for (idx_t i = (idx_t)cached_upto + 1; i <= n; ++i) {
        unsigned __int128 term1 = (unsigned __int128)cache[i-1] * (unsigned __int128)(2*(3*i+4));
        unsigned __int128 term2 = (unsigned __int128)cache[i-2] * (unsigned __int128)(10*i-4);
        unsigned __int128 term3 = (unsigned __int128)cache[i-3] * (unsigned __int128)(4*(i-2));
        unsigned __int128 term4 = (unsigned __int128)cache[i-4] * (unsigned __int128)(i-3);
        unsigned __int128 term5 = (unsigned __int128)cache[i-5] * (unsigned __int128)(2*(i-5));
        unsigned __int128 num   = term1 - term2 + term3 - term4 + term5;
        cache[i] = (seq_t)(num / (unsigned __int128)(i+3));
    }
    cached_upto = (size_t)n;
    return cache[(size_t)n];
#endif
}
