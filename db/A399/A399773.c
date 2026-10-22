/*
 * A399773  a(1) = a(2) = 1; for n > 2, a(n) = a(a(n-1)) + a(n-a(n-1)) +
 *          (-1)^n.
 * https://oeis.org/A399773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(1)=a(2)=1; for n>2, a(n)=a(a(n‑1))+a(n-a(n‑1))+(-1)^n
 * Range:     u8 n <= 858, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* index 0 unused, cache[i] = a(i) */
    static idx_t cap  = 0;            /* current allocation size */
    static idx_t last = 0;            /* highest computed index */

    if (n == 0) return 0;            /* undefined but safe */

    /* grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();     /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = (idx_t)new_cap;
    }

    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = 1;
        } else {
            seq_t prev_val = cache[i - 1];
            idx_t idx1 = (idx_t)prev_val;
            idx_t idx2 = i - (idx_t)prev_val;
            seq_t sum = cache[idx1] + cache[idx2];
            if ((i & 1) == 0)
                sum += 1;          /* (-1)^n is +1 for even n */
            else
                sum -= 1;          /* (-1)^n is -1 for odd n  */
            cache[i] = sum;
        }
    }
    last = n;
    return cache[n];
}
