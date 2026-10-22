/*
 * A361567  Expansion of e.g.f. exp(x^2/2 * (1+x)^2).
 * https://oeis.org/A361567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-2) + 3*(n-2)*(n-1)*a(n-3) +
 *            2*(n-3)*(n-2)*(n-1)*a(n-4) (n >= 4) Initial values (offset 0):
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t  capacity = 0;
    static idx_t  size     = 0;          /* number of terms already stored */

    /* grow cache if necessary */
    if (n >= size) {
        idx_t new_cap = capacity ? capacity : 8;
        if (new_cap <= n) {
            /* double until it can hold n */
            while (new_cap <= n)
                new_cap <<= 1;
        }
        if (new_cap > capacity) {
            seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!tmp) abort();
            for (idx_t i = 0; i < size; ++i)
                tmp[i] = cache[i];
            free(cache);
            cache = tmp;
            capacity = new_cap;
        }

        /* initialize first four terms once */
        if (size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)6;
            size = 4;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            /* for i < 4 the values are already set */
            if (i < 4) continue;

            seq_t t1 = ((seq_t)(i - 1)) * cache[i - 2];
            seq_t t2 = ((seq_t)3 * (seq_t)(i - 2) * (seq_t)(i - 1)) * cache[i - 3];
            seq_t t3 = ((seq_t)2 * (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1)) * cache[i - 4];
            cache[i] = t1 + t2 + t3;
        }
        size = n + 1;
    }

    return cache[n];
}
