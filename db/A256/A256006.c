/*
 * A256006  Recurrence: a(n) = Sum_{k=0..n-1} a(k)*C(n+1,k), a(0)=1.
 * https://oeis.org/A256006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} a(k)*C(n+1,k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;          /* number of terms currently stored   */
    static size_t capacity = 0;         /* allocated size of cache            */

    if (!cache) {
        capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)1;           /* a(0) */
        computed = 1;
    }

    if (n >= computed) {
        if (capacity <= (size_t)n) {
            size_t newcap = capacity ? capacity : 1;
            while (newcap <= (size_t)n) newcap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
            capacity = newcap;
        }

        for (idx_t i = computed; i <= n; ++i) {
            seq_t sum   = 0;
            seq_t binom = 1;              /* C(i+1,0) */
            idx_t m = i + 1;

            for (idx_t k = 0; k < i; ++k) {
                sum += cache[k] * binom;

                seq_t num = binom * ( (seq_t)(m - k) );
                binom     = num / ( (seq_t)(k + 1) );
            }
            cache[i] = sum;
        }
        computed = n + 1;
    }

    return cache[n];
}
