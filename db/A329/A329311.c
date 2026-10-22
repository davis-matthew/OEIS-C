/*
 * A329311  a(n) is the product of the numbers k such that a(n-2*k) = a(n-k)
 *          and 0 < n-2*k < n-k < n.
 * https://oeis.org/A329311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 39, u32 n <= 108, u64 n <= 300,
 *            u128 n <= 895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329311:
   a(n) = product of all k such that 0 < k < n/2 and a(n‑2k) = a(n‑k).
   The product is 1 when no such k exists.  a(1) = 1, a(2) = 1 … */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* a(1) .. a(cache_size) */
    static idx_t cache_cap = 0;      /* allocated capacity     */
    static idx_t cache_size = 0;     /* current number stored  */

    if (n < 1)
        return (seq_t)0;             /* undefined for n < 1 */

    /* Allocate or grow the buffer so that it can hold a(1)..a(n) */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < n)
            new_cap <<= 1;
        /* cast realloc result to seq_t* for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                     /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values from cache_size+1 up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        idx_t N = i + 1;               /* actual sequence index, N ≥ 1 */
        seq_t prod = (seq_t)1;          /* product of qualifying k's */

        for (idx_t k = 1; (k << 1) < N; ++k) {
            idx_t idx1 = N - (k << 1);   /* n-2k  ≥ 1   */
            idx_t idx2 = N - k;          /* n-k   ≥ 1   */
            if (cache[idx1 - 1] == cache[idx2 - 1])
                prod *= (seq_t)k;
        }

        cache[i] = prod;
    }

    cache_size = n;
    return cache[n - 1];
}
