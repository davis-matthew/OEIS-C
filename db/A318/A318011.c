/*
 * A318011  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A318011
 * a(1)=4, a(2)=28, a(3)=118, a(4)=508, a(5)=2130, a(6)=8971, a(7)=37700,
 * a(8)=158613, a(9)=667178, a(10)=2806643, a(11)=11806334, a(12)=49664719,
 * a(13)=208919728, a(14)=878843413
 * a(n)=3*a(n-1)+5*a(n-2)+3*a(n-4)-4*a(n-5)-11*a(n-6)-6*a(n-7)
 *      -a(n-8)-4*a(n-9)-4*a(n-10)-a(n-11)   (n>14)
 */

#include <stdlib.h>

/* 14 initial terms (index 0 unused) */
static const seq_t base[15] = {
    0ULL,                       /* dummy for index 0 */
    4ULL, 28ULL, 118ULL, 508ULL, 2130ULL, 8971ULL, 37700ULL, 158613ULL,
    667178ULL, 2806643ULL, 11806334ULL, 49664719ULL,
    208919728ULL, 878843413ULL
};

seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* sequence starts at index 1 */

    /* static cache and its highest computed index */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        max_n = 14;
        cache = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */
        for (idx_t i = 1; i <= max_n; ++i)
            cache[i] = base[i];
        cache[0] = 0;          /* unused slot for 0‑th index */
    }

    /* extend cache if needed */
    if (n > max_n) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failure */
        cache = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] =
                (seq_t)3 * cache[i-1] +
                (seq_t)5 * cache[i-2] +
                (seq_t)3 * cache[i-4] -
                (seq_t)4 * cache[i-5] -
                (seq_t)11 * cache[i-6] -
                (seq_t)6 * cache[i-7] -
                cache[i-8] -
                (seq_t)4 * cache[i-9] -
                (seq_t)4 * cache[i-10] -
                cache[i-11];
        }
        max_n = n;
    }

    return cache[n];
}
