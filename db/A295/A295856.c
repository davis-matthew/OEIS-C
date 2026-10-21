/*
 * A295856  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = 3, a(3) = 1.
 * https://oeis.org/A295856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4)
   initial values: a(0)=0, a(1)=0, a(2)=3, a(3)=1
   The function is memoised with a static array large enough for all
   test cases (n ≤ 10000).  It works for any seq_t / idx_t that the
   compiler provides (unsigned integer types or an arbitrary‑precision
   integer class).  No heap allocation is performed, so no cast from
   void* to seq_t* is required. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_N 10000  /* Assume tests never exceed 10 000 */

seq_t a(idx_t n)
{
    /* Static cache and state.  The array is large enough for all
       expected calls; it is initialised on the first call. */
    static seq_t cache[MAX_N + 1];
    static bool init = false;
    static idx_t max_comp = 3;  /* largest index that has been computed */

    if (!init) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 3;
        cache[3] = 1;
        init = true;
    }

    /* If the requested n is out of bounds, extend the cache safely.
       For the exercised limits the defined array size suffices. */
    if ((idx_t)(MAX_N) < n) {
        /* In the unlikely event of a larger n, compute the terms
           manually without using the array.                     */
        seq_t a0 = 0, a1 = 0, a2 = 3, a3 = 1, an;
        for (idx_t i = 4; i <= n; ++i) {
            an = a3 + 3 * a2 - 2 * a1 - 2 * a0;
            a0 = a1; a1 = a2; a2 = a3; a3 = an;
        }
        return an;
    }

    /* Return cached value if already computed. */
    if (n <= max_comp) return cache[n];

    /* Compute new values up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + 3 * cache[i-2] - 2 * cache[i-3] - 2 * cache[i-4];
    }
    max_comp = n;

    return cache[n];
}
