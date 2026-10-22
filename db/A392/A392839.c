/*
 * A392839  a(n) = (F(4*n + 3) - F(4*n - 1) - 5*n + 9)/5, where F(n) =
 *          A000045(n).
 * https://oeis.org/A392839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(4*n + 3) - F(4*n - 1) - 5*n + 9)/5 Recurrence: a(n) =
 *            9*a(n-1) - 16*a(n-2) + 9*a(n-3) - a(n-4), n > 3 Initial values:
 *            a(0)=2, a(1)=3, a(2)=15, a(3)=103
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cached = NULL;
    static idx_t cap = 0;          /* current allocated size */
    static idx_t computed = (idx_t)-1;  /* highest index that has been computed */

    /* Handle small n directly. No dynamic allocation needed. */
    if (n <= 3) {
        static const seq_t init[4] = {2, 3, 15, 103};
        return init[n];
    }

    /* One-time allocation and initialization of the cache. */
    if (cached == NULL) {
        cap = 4;
        cached = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cached) abort();       /* allocation failure – terminate */

        cached[0] = 2;
        cached[1] = 3;
        cached[2] = 15;
        cached[3] = 103;
        computed = 3;
    }

    /* Compute all missing terms up to n. */
    while (computed < n) {
        idx_t k = computed + 1;               /* index to compute */

        /* Ensure sufficient capacity. */
        if (k >= cap) {
            size_t newcap = cap << 1;
            if (newcap < cap + 1) newcap = cap + 1;
            seq_t *tmp = (seq_t *)realloc(cached, sizeof(seq_t) * newcap);
            if (!tmp) abort();
            cached = tmp;
            cap = newcap;
        }

        /* Use the recurrence:
           a(k) = 9*a(k-1) - 16*a(k-2) + 9*a(k-3) - a(k-4)
           We rearrange to avoid intermediate underflow:
           a(k) = (9*a(k-1)+9*a(k-3)) - (16*a(k-2)+a(k-4))
        */
        seq_t sum1 = 9 * cached[k - 1] + 9 * cached[k - 3];
        seq_t sum2 = 16 * cached[k - 2] + cached[k - 4];
        cached[k] = sum1 - sum2;

        computed = k;
    }

    return cached[n];
}
