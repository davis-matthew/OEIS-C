/*
 * A301424  Sums of positive coefficients of generalized Chebyshev polynomials
 *          of the first kind, for a family of 7 data.
 * https://oeis.org/A301424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2)=7; a(3)=64; a(4)=609; a(5)=5846; a(6)=56161;
 *            a(7)=539540; a(8)=5183417; For n>=9: a(n) = 8*a(n-1) + 14*a(n-2)
 *            + 14*a(n-3) - 14*a(n-5) - 14*a(n-6) - 6*a(n-7) - a(n-8)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* total capacity (cache[0] unused) */
    static size_t last  = 0;   /* highest index whose value is valid */

    /* Grow cache if necessary */
    if (!cache || cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* Initialise the first 8 terms (if the capacity allows) */
        if (cap > 1) cache[1] = (seq_t)1;
        if (cap > 2) cache[2] = (seq_t)7;
        if (cap > 3) cache[3] = (seq_t)64;
        if (cap > 4) cache[4] = (seq_t)609;
        if (cap > 5) cache[5] = (seq_t)5846;
        if (cap > 6) cache[6] = (seq_t)56161;
        if (cap > 7) cache[7] = (seq_t)539540;
        if (cap > 8) cache[8] = (seq_t)5183417;

        /* Set the last computed index based on how many terms we have */
        last = cap - 1 < 8 ? cap - 1 : 8;
    }

    /* Return early if the requested value is already known */
    if ((size_t)n <= last) return cache[(size_t)n];

    /* Compute new values up to n */
    size_t i = last + 1;
    if (i < 9) i = 9;          /* recurrence applies only for n >= 9 */

    for (; i <= (size_t)n; ++i) {
        seq_t pos = (seq_t)8 * cache[i-1];
        pos += (seq_t)14 * cache[i-2];
        pos += (seq_t)14 * cache[i-3];

        seq_t neg = (seq_t)14 * cache[i-5];
        neg += (seq_t)14 * cache[i-6];
        neg += (seq_t)6  * cache[i-7];
        neg += cache[i-8];

        cache[i] = pos - neg;
    }

    if (last < (size_t)n) last = (size_t)n;
    return cache[(size_t)n];
}
