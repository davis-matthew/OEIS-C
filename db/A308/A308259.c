/*
 * A308259  a(n) is equal to the sum of the factorials of the digits of
 *          a(n-1), initial term is 3.
 * https://oeis.org/A308259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 3; a(n) = sum of factorials of the digits of a(n-1)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* array of computed values */
static size_t cache_capacity = 0; /* allocated size of cache   */
static idx_t last_computed   = 0; /* number of terms stored   */

static const seq_t digit_fact[10] =
    {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined for n = 0; safe default */

    /* Allocate or enlarge cache if necessary */
    if (cache == NULL || (size_t)n > cache_capacity) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)       /* allocation failure guard   */
            return 0;
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Initialize first term if cache empty */
    if (last_computed == 0) {
        cache[0] = 3;         /* a(1) = 3 */
        last_computed = 1;
    }

    /* Compute and store missing terms up to n */
    for (idx_t idx = last_computed + 1; idx <= n; ++idx) {
        seq_t prev = cache[idx - 2];   /* a(idx-1) is stored at idx-2 */
        seq_t sum  = 0;
        seq_t tmp  = prev;
        while (tmp > 0) {
            int digit = (int)(tmp % 10);
            sum += digit_fact[digit];
            tmp /= 10;
        }
        cache[idx - 1] = sum;
    }

    last_computed = n;
    return cache[n - 1];
}
