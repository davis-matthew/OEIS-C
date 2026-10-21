/*
 * A284547  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 998", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-8) - 2*a(n-9) for n > 25, with a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=11, a(4)=21, a(5)=43, a(6)=85, a(7)=171,
 *            a(8)=341, a(9)=683, a(10)=1365, a(11)=2987, … (values 12..25 are
 *            generated from the generating‐function)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* coefficients N[n] of the numerator of the generating function */
static const int64_t N[26] = {
    1,          /* x^0  */
    1,          /* x^1  */
   -1,          /* x^2  */
    1,          /* x^3  */
   -1,          /* x^4  */
    1,          /* x^5  */
   -1,          /* x^6  */
    1,          /* x^7  */
   -2,          /* x^8  */
    0,          /* x^9  */
    0,          /* x^10 */
   256,         /* x^11 */
    0,          /* x^12 */
  -512,         /* x^13 */
 1536,         /* x^14 */
  -256,        /* x^15 */
   512,         /* x^16 */
 1024,         /* x^17 */
    0,          /* x^18 */
    0,          /* x^19 */
   768,         /* x^20 */
  -768,         /* x^21 */
 6400,         /* x^22 */
    0,          /* x^23 */
   512,         /* x^24 */
 1024          /* x^25 */
};

/* The sequence. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* computed values */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t computed = 0;           /* next index to compute */
    static int initialized = 0;

    /* Ensure cache has enough space */
    if (!initialized) {
        size_t need = (n >= 26 ? n + 1 : 26);
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_cap = need;

        /* Compute the first 26 terms using the recurrence that includes N[n] */
        for (int i = 0; i < 26; ++i) {
            seq_t t1 = 0;
            seq_t sub = 0;

            if (i >= 1) t1 = 2 * cache[i - 1];
            if (i >= 8) t1 += cache[i - 8];
            if (i >= 9) sub = 2 * cache[i - 9];

            if (N[i] < 0) {
                /* subtract |N[i]| */
                sub += (seq_t)(-N[i]);
            } else {
                /* add N[i] (zero is a no‑op) */
                t1 += (seq_t)N[i];
            }

            cache[i] = t1 - sub;
        }

        computed = 26;
        initialized = 1;
    } else if (cache_cap < n + 1) {
        /* Grow the cache if necessary */
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute remaining terms when required */
    if (computed <= n) {
        for (idx_t i = computed; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + cache[i - 8] - 2 * cache[i - 9];
        }
        computed = n + 1;
    }

    return cache[n];
}
