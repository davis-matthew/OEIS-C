/*
 * A252722  Number of (3+2) X (n+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - 2*a(n-5) for n>6; a(1)=54, a(2)=56,
 *            a(3)=62, a(4)=70, a(5)=86, a(6)=110
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 82, u64 n <= 178,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A252722 */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;     /* largest index computed so far */

    if (n == 0) /* undefined, but safe return */
        return (seq_t)0;

    size_t nn = (size_t)n;

    /* Initialise cache with base values if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((7) * sizeof(seq_t));   /* indices 0..6 (0 unused) */
        if (!cache)
            return (seq_t)0;                           /* memory allocation failure */
        cache[0] = 0;
        cache[1] = (seq_t)54;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)62;
        cache[4] = (seq_t)70;
        cache[5] = (seq_t)86;
        cache[6] = (seq_t)110;
        cache_cnt = 6;
    }

    /* If already computed, return immediately */
    if (nn <= cache_cnt)
        return cache[nn];

    /* Grow cache to accommodate index nn */
    seq_t *tmp = (seq_t *)realloc(cache, (nn + 1) * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;   /* memory allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_cnt + 1; i <= nn; ++i) {
        /* a(i) = a(i-2) + 2*a(i-3) - 2*a(i-5)   (valid for i >= 7) */
        cache[i] = cache[i - 2] + 2 * cache[i - 3] - 2 * cache[i - 5];
    }

    cache_cnt = nn;
    return cache[nn];
}
