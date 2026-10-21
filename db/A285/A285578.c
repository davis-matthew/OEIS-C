/*
 * A285578  a(1) = a(3) = a(4) = 2, a(2) = 1; a(n) = a(a(n-2)) + a(n-a(n-4)-2)
 *          for n > 4.
 * https://oeis.org/A285578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = a(3) = a(4) = 2, a(2) = 1;
   a(n) = a(a(n-2)) + a(n - a(n-4) - 2)  for n > 4   */

/* The routine is fully self‑contained; it allocates a temporary
   array of seq_t large enough to hold all required values.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) abort();          /* sequence is defined for n ≥ 1 */

    /* Allocate working array indexed from 1 to n */
    seq_t *cache = NULL;
#ifdef __cplusplus
    cache = new seq_t[n + 1];
    if (!cache) abort();          /* bad_alloc would be thrown otherwise */
#else
    cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cache) abort();
#endif

    /* Initialise the first four terms */
    cache[1] = 2;
    if (n >= 2) cache[2] = 1;
    if (n >= 3) cache[3] = 2;
    if (n >= 4) cache[4] = 2;

    /* Compute remaining terms */
    for (idx_t i = 5; i <= n; ++i) {
        idx_t idx_a_nm2 = (idx_t)cache[i - 2];
        idx_t idx_a_nm4 = (idx_t)cache[i - 4];
        idx_t idx_n_a_nm4_2 = i - idx_a_nm4 - 2;   /* always ≥ 1 */

        cache[i] = cache[idx_a_nm2] + cache[idx_n_a_nm4_2];
    }

    seq_t result = cache[n];

#ifndef __cplusplus
    free(cache);
#else
    delete[] cache;
#endif
    return result;
}
