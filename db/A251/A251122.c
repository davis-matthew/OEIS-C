/*
 * A251122  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5); Initial
 *            values: a(1)=21, a(2)=40, a(3)=69, a(4)=117, a(5)=193
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *CACHE = NULL;
static idx_t CACHE_SIZE = 0;
static idx_t LAST_COMPUTED = 0;
static const seq_t BASE[6] = {0, 21, 40, 69, 117, 193};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Ensure we have space for indices up to n */
    ensure_cache(n, &CACHE, &CACHE_SIZE);

    /* Initialize the first five values if they are not already set */
    if (LAST_COMPUTED < 5) {
        idx_t up = 5;
        if (up > n) up = n;
        for (idx_t i = 1; i <= up; ++i) CACHE[i] = BASE[i];
        LAST_COMPUTED = up;
    }

    /* Compute the sequence up to n if necessary */
    if (LAST_COMPUTED < n) {
        for (idx_t i = LAST_COMPUTED + 1; i <= n; ++i) {
            /* Apply the recurrence
               a(i) = 4*a(i-1) - 5*a(i-2) + a(i-3) + 2*a(i-4) - a(i-5) */
            seq_t term = 4*CACHE[i-1] + CACHE[i-3] + 2*CACHE[i-4];
            term -= 5*CACHE[i-2] + CACHE[i-5];
            CACHE[i] = term;
        }
        LAST_COMPUTED = n;
    }

    return CACHE[n];
}
