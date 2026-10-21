/*
 * A275454  G.f.: 3F2([1/9, 4/9, 8/9], [2/3, 1], 729 x).
 * https://oeis.org/A275454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A275454
   G.f.: 3F2([1/9,4/9,8/9],[2/3,1],729*x)
   Recurrence: n^2*(3n-1)*a(n) = 3*(9n-5)*(9n-8)*(9n-1)*a(n-1)
   Initial value: a(0) = 1   (offset 0)
*/
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t  cache_len = 0;  /* number of cached elements */

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) = 1 if empty */
    if (cache_len == 0) {
        cache_len = 1;
        cache = alloc_seq_array(cache_len);
        cache[0] = (seq_t)1;
    }

    /* Extend cache up to index n if necessary */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        /* allocate new cache with space for indices 0..n */
        seq_t *new_cache = alloc_seq_array(n + 1);
        /* copy existing values */
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_len = n + 1;

        /* Compute missing terms using the recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            /* Numerator: 3 * (9i-5) * (9i-8) * (9i-1) */
            seq_t num = (seq_t)3 * (((seq_t)9 * i) - 5)
                        * (((seq_t)9 * i) - 8)
                        * (((seq_t)9 * i) - 1);
            /* Denominator: i^2 * (3i-1) */
            seq_t den = ((seq_t)i) * ((seq_t)i) * (((seq_t)3 * i) - 1);

            /* Reduce common factors between numerator and denominator */
            seq_t g1 = gcd(num, den);
            num /= g1;
            den /= g1;

            /* Reduce remaining denominator with prev */
            seq_t g2 = gcd(prev, den);
            prev /= g2;
            den /= g2;

            /* After reduction the denominator should be 1.
               Compute the new term safely. */
            seq_t new_val;
            if (den == (seq_t)1) {
                new_val = prev * num;
            } else {
                new_val = (prev * num) / den;   /* divisible by construction */
            }

            cache[i] = new_val;
        }
    }

    return cache[n];
}
