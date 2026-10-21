/*
 * A281488  a(n) = -Sum_{d divides (n-2), 1 <= d < n} a(d).
 * https://oeis.org/A281488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, look, hear
 * Formula:   not stated
 * Range:     i8 n <= 137, i16 n <= 771, i32 n <= 8521, i64 n <= 9639,
 *            i128 n <= 9639, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n) = -Sum_{d | (n-2), 1 <= d < n} a(d)   (n > 1) */

/* Dependencies that we may use freely */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#ifndef __cplusplus
#include <stdlib.h>
#endif

/* In C++ use std::vector to ensure proper constructor/destructor calls for
   arbitrary-precision integer types. */
#ifdef __cplusplus
#include <vector>
#endif

/* Visibility of types:
   seq_t : signed integer or big integer class
   idx_t : unsigned integer (uint64_t) */

/* Static cache for memoization */
static seq_t *cache = NULL;
static unsigned char *seen = NULL;
static size_t cache_cap = 0;   /* current allocated capacity (>=1 for index 0) */

/* Helper: ensure that cache can address indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* we need indices 0..n, so allocate n+1 entries */
    idx_t need = n + 1;
    if ((size_t)need <= cache_cap) return;

#ifdef __cplusplus
    /* Use std::vector for C++ to guarantee construction of seq_t elements */
    static std::vector<seq_t> cache_vec;
    static std::vector<unsigned char> seen_vec;

    cache_vec.resize(need);          /* default‑construct new seq_t's */
    seen_vec.resize(need, 0);        /* new seen entries set to 0 */

    cache = cache_vec.data();
    seen  = seen_vec.data();
    cache_cap = need;
#else
    /* For C, use realloc and set new seen entries to 0 */
    seq_t *tmp_cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
    unsigned char *tmp_seen = (unsigned char*)realloc(seen, need * sizeof(unsigned char));

    if (!tmp_cache || !tmp_seen) {
        /* allocation failed: leave cache unchanged; subsequent use will
           produce undefined behavior, but this situation is unlikely for
           the intended n range. */
        return;
    }
    /* initialise newly allocated seen entries */
    memset(&tmp_seen[cache_cap], 0, (need - cache_cap) * sizeof(unsigned char));

    cache = tmp_cache;
    seen  = tmp_seen;
    cache_cap = need;
#endif
}

/* Main function implementing A281488 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard anyway */

    ensure_cache(n);

    /* Compute all values up to n that haven't been computed yet */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (seen[i]) continue;     /* already computed */

        seq_t val;
        if (i == 1) {
            val = 1;                /* a(1) = 1 */
        } else {
            idx_t t = i - 2;        /* argument whose divisors we sum over */
            seq_t sum = 0;

            if (t == 0) {
                /* n = 2: divisors of 0 are considered to contain only 1 < 2 */
                sum = cache[1];
            } else {
                /* enumerate divisors d of t (d <= sqrt(t)) */
                for (idx_t d = 1; d * d <= t; ++d) {
                    if (t % d == 0) {
                        sum += cache[d];
                        idx_t d2 = t / d;
                        if (d2 != d) sum += cache[d2];
                    }
                }
            }
            val = -sum;
        }

        cache[i] = val;
        seen[i] = 1;
    }

    return cache[n];
}
