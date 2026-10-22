/*
 * A334605  Denominator of Sum_{k=1..n} (-1)^(k+1)/k^6.
 * https://oeis.org/A334605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of the partial sum S(n) = Σ_{k=1..n}
 *            (-1)^{k+1} / k^6, reduced to lowest terms. The function memoises
 *            results so that calls with earlier or later indices share
 *            computed terms
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a running fraction p/q and adds
   each term incrementally.  All arithmetic is performed with
   the seq_t type, which may be an unsigned integer of any size
   or an arbitrary‑precision integer class.  A simple dynamic
   array stores the denominators for indices that have already
   been computed.  No external libraries beyond the standard
   C++/C headers and the provided gcd helper are required. */

#include <stdlib.h>          /* malloc, realloc */
#include <stdbool.h>         /* bool, true, false */
#include "../utils/gcd.h"    /* gcd(seq_t, seq_t) */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *den_cache  = NULL;   /* array of denominators, 1‑based */
    static idx_t  cache_cap  = 0;      /* allocated capacity (max index) */
    static idx_t  computed   = 0;      /* largest n computed so far */

    /* Running sum stored as numerator (abs), sign flag, and denominator */
    static seq_t cur_num_abs = 0;      /* absolute value of numerator */
    static bool   cur_neg     = false; /* true if numerator is negative */
    static seq_t cur_den      = 1;     /* denominator (always positive) */

    /* Safety guard for indices below the sequence offset */
    if (n < 1) return 0;

    /* Allocate or grow the cache if necessary */
    if (n > cache_cap) {
        /* Determine new capacity: round up to the next power of two for speed */
        idx_t new_cap = (n > 64 ? n : 64);          /* start with a modest size */
        /* Grow until new_cap >= n */
        while (new_cap < n) new_cap <<= 1;

        seq_t *tmp;
        if (den_cache == NULL) {
            tmp = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(den_cache,
                                   (new_cap + 1) * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* Memory allocation failure – return 0 as a safe fallback */
            return 0;
        }
        den_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already computed */
    if (n > computed) {
        for (idx_t k = computed + 1; k <= n; ++k) {
            /* k^6 as seq_t */
            seq_t kseq = (seq_t)k;
            seq_t k2   = kseq * kseq;   /* k^2 */
            seq_t k4   = k2   * k2;     /* k^4 */
            seq_t k6   = k4   * k2;     /* k^6 */

            /* Sign of the new term: + for odd k, - for even k */
            bool term_pos = (k & 1ULL) != 0;   /* true if +1/k^6 */

            /* Reduce k6 against current denominator */
            seq_t g = gcd(cur_den, k6);
            seq_t mult1 = cur_den / g;   /* cur_den / g  (positive) */
            seq_t mult2 = k6      / g;   /* k6 / g       (positive) */

            /* Current fraction scaled to common denominator: t1 = cur_num_abs * mult2 */
            seq_t t1 = cur_num_abs * mult2;
            /* Sign of t1 matches current fraction sign (cur_neg) */

            /* Term to add scaled to common denominator: t2 = mult1 */
            seq_t t2 = mult1;
            /* Sign of t2 matches term_pos (positive if term_pos else negative) */

            /* Combine signed numbers: decide new absolute value and sign */
            seq_t new_abs;
            bool   new_neg;

            if (cur_neg == !term_pos) {  /* signs differ */
                if (t1 >= t2) {
                    new_abs = t1 - t2;
                    new_neg = cur_neg;          /* same as the larger part's sign */
                } else {
                    new_abs = t2 - t1;
                    new_neg = !term_pos;        /* sign of the larger part */
                }
            } else {                      /* signs equal – add magnitudes */
                new_abs = t1 + t2;
                new_neg = cur_neg;           /* sign of the common part */
            }

            /* New denominator before reduction is lcm(cur_den, k6) */
            seq_t new_den = mult1 * k6;     /* (cur_den/g) * k6 */

            /* Reduce the fraction */
            seq_t g2 = gcd(new_abs, new_den);
            new_abs = new_abs / g2;
            new_den = new_den / g2;

            /* Update running values */
            cur_num_abs = new_abs;
            cur_neg     = new_neg;
            cur_den     = new_den;

            /* Store the denominator for this index */
            den_cache[(size_t)k] = new_den;
        }
        computed = n;
    }

    return den_cache[(size_t)n];
}
