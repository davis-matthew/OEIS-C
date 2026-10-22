/*
 * A373113  For n >= 1, a(n) = Sum_{i = n..(n + A000005(n) - 1)} i.
 * https://oeis.org/A373113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n)*(A000005(n)-1)/2 + n*A000005(n)
 * Range:     u8 n <= 29, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = d(n)*(d(n)-1)/2 + n*d(n) ,  where d(n)=A000005(n) */

#include <stdlib.h>
#include <stdint.h>

/* Count the number of positive divisors of m (m >= 1). */
static uint32_t divisor_count(idx_t m)
{
    uint32_t cnt = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            ++cnt;
            if (i != m / i)
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static uint32_t *divcnt = NULL;   /* divcnt[k] = d(k) for k <= max_n */

    if (n == 0)   /* offset is 1; undefined below this */
        return (seq_t)0;

    /* Extend the cache if necessary. */
    if (n > max_n) {
        size_t new_max = (size_t)n + 1;   /* allocate up to n inclusive */
        void *p = realloc(divcnt, new_max * sizeof(uint32_t));
        if (!p)
            abort();
        divcnt = (uint32_t *)p;
        for (size_t k = max_n + 1; k <= n; ++k)
            divcnt[k] = divisor_count((idx_t)k);
        max_n = n;
    }

    seq_t d = (seq_t)divcnt[n];
    seq_t part = d * (d - 1) / 2;     /* d*(d-1)/2 */
    seq_t res  = part + (seq_t)n * d; /* part + n*d */
    return res;
}
