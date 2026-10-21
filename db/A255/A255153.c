/*
 * A255153  Number of (n+2) X (2+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 1 and no row sum 0 or 1 and
 *          no column sum 0 or 1.
 * https://oeis.org/A255153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255153: number of (n+2)x(2+2) binary arrays with the given restrictions.
   The sequence satisfies the recurrence
       a(n) = a(n-1) + 2*a(n-2) + 7*a(n-3) + 4*a(n-4) + a(n-6)
              - 6*a(n-8) - 6*a(n-9) - 3*a(n-10) - a(n-11),
   for n ≥ 12, with the initial values
       a(1)=62,  a(2)=158,  a(3)=459,  a(4)=1285,  a(5)=3553,  a(6)=9978,
       a(7)=27903, a(8)=77798, a(9)=217554, a(10)=608257, a(11)=1699201. */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n); /* forward declaration for compilers that need it */

/* Memoized recurrence evaluation. */
static seq_t *cache = NULL; /* cache[0] unused, cache[1] = a(1), etc. */
static idx_t cache_size = 0; /* number of elements stored (>=0) */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    /* Ensure the cache can hold values up to n. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory; return 0 to satisfy the interface */
            return 0;
        }
        cache = new_cache;

        /* Compute values from old cache_size up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 1: val = 62; break;
                case 2: val = 158; break;
                case 3: val = 459; break;
                case 4: val = 1285; break;
                case 5: val = 3553; break;
                case 6: val = 9978; break;
                case 7: val = 27903; break;
                case 8: val = 77798; break;
                case 9: val = 217554; break;
                case 10: val = 608257; break;
                case 11: val = 1699201; break;
                default: /* recurrence for i >= 12 */
                    val = cache[i - 1];
                    val += cache[i - 2] * 2;
                    val += cache[i - 3] * 7;
                    val += cache[i - 4] * 4;
                    val += cache[i - 6];
                    val -= cache[i - 8] * 6;
                    val -= cache[i - 9] * 6;
                    val -= cache[i - 10] * 3;
                    val -= cache[i - 11];
                    break;
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
