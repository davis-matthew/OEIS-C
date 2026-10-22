/*
 * A358987  Omit the trailing 5 from double factorial of odd numbers
 *          (A001147(n)).
 * https://oeis.org/A358987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   A001147(n) = ∏_{k=0}^{n-1} (2k+1) (double factorial of odd
 *            numbers) a(n) = (A001147(n) - 5) / 10 for n > 2 a(0) = 1, a(1) =
 *            1, a(2) = 3 Computes a(n) for any n ≥ 0 using memoised
 *            incremental products
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;          /* array of a(i) values */
    static size_t cap   = 0;             /* allocated capacity of cache   */
    static size_t size  = 0;             /* number of valid entries        */
    static seq_t prod   = (seq_t)1;      /* ∏_{k=0}^{i-1} (2k+1) for last i-1 */

    /* Ensure the cache can hold values up to n */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first entry (a(0) = 1) if not already done */
    if (size == 0) {
        cache[0] = (seq_t)1;
        size = 1;
        /* prod already 1, which equals ∏_{k=0}^{-1} ...  */
    }

    /* Return cached value if already computed */
    if (n < size) return cache[n];

    /* Extend the cache up to the requested index */
    for (idx_t i = size; i <= n; ++i) {
        /* Compute product for index i: prod *= (2*i-1)  */
        seq_t odd = (seq_t)(2 * i - 1);
        prod *= odd;

        seq_t val;
        if (i <= 2) {
            /* Handle the first three terms explicitly */
            if (i == 1)      val = (seq_t)1;
            else             val = (seq_t)3;   /* i == 2 */
        } else {
            /* a(i) = (prod - 5) / 10  for i > 2 */
            val = (prod - (seq_t)5) / (seq_t)10;
        }
        cache[i] = val;
    }
    size = n + 1;
    return cache[n];
}
