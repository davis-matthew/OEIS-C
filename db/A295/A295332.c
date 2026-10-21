/*
 * A295332  Denominators of the continued fraction convergents to sqrt(13)/2 =
 *          A295330.
 * https://oeis.org/A295332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = 1298*a(n-6) - a(n-12) for n ≥ 12 with initial values:
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=71, a(4)=289, a(5)=360, a(6)=1009,
 *            a(7)=1369, a(8)=6485, a(9)=92159, a(10)=375121, a(11)=467280
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *buf = NULL;
    static size_t buf_len = 0;

    /* Initial six terms (indices 0..11) */
    static const seq_t init[12] =
    {
        (seq_t)1,
        (seq_t)1,
        (seq_t)5,
        (seq_t)71,
        (seq_t)289,
        (seq_t)360,
        (seq_t)1009,
        (seq_t)1369,
        (seq_t)6485,
        (seq_t)92159,
        (seq_t)375121,
        (seq_t)467280
    };

    /* Allocate and initialize cache on first call */
    if (!buf)
    {
        buf_len = 12;
        buf = (seq_t *)malloc(sizeof(seq_t) * buf_len);
        for (size_t i = 0; i < buf_len; ++i)
            buf[i] = init[i];
    }

    /* Return already computed value if available */
    if ((idx_t)buf_len > n)
        return buf[n];

    /* Extend cache to include value at index n */
    idx_t new_len = n + 1;                /* new cache length */
    buf = (seq_t *)realloc(buf, sizeof(seq_t) * new_len);

    /* Compute missing terms using the recurrence */
    for (idx_t i = (idx_t)buf_len; i <= n; ++i)
        buf[i] = (seq_t)(1298 * buf[i - 6] - buf[i - 12]);

    buf_len = new_len;
    return buf[n];
}
