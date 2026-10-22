/*
 * A307704  Expansion of (1/(1 - x)) * Sum_{k>=1} (-x)^k/(1 - (-x)^k).
 * https://oeis.org/A307704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 8399, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307704
 * Formula: a(n) = Sum_{k = 1..n} (-1)^k * A000005(k)
 *          (A000005(k) = number of divisors of k)
 */
#include <stdlib.h>

/* External linkage as required – no main() function is defined.  */
seq_t a(idx_t n)
{
    /* Static cache: pref[i] stores the value of a(i).
       pref[0] is defined as 0 for convenience. */
    static seq_t *pref = NULL;
    static idx_t cache_n = 0;      /* largest index already stored */

    if (n == 0)
        return 0;                  /* undefined for n = 0, harmless */

    /* Extend the cache if necessary. */
    if (n > cache_n) {
        /* Reallocate enough room for indices up to n. */
        seq_t *tmp = (seq_t *)realloc(pref, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;              /* allocation failed – return 0 */
        pref = tmp;

        /* Initialise the 0‑th element if this is the first call. */
        if (cache_n == 0)
            pref[0] = (seq_t)0;

        /* Compute a(i) for each new i = cache_n+1 .. n. */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            /* Count divisors of i.  A small O(√i) loop is fast enough
               for the required limits (n ≤ ~10 000). */
            seq_t cnt = (seq_t)0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    ++cnt;                        /* divisor d */
                    if (d != i / d)
                        ++cnt;                    /* divisor i/d if distinct */
                }
            }

            /* Sign factor: (-1)^i – negative for odd i. */
            seq_t sign = (i & 1) ? (seq_t)-1 : (seq_t)1;

            /* Cumulative sum. */
            pref[i] = pref[i - 1] + sign * cnt;
        }

        cache_n = n;   /* mark that pref[0..n] are valid now */
    }

    return pref[n];
}
