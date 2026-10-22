/*
 * A301967  Number of 5Xn 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301967
 * Recurrence (n > 17):
 *   a(n) = 3*a(n-1) - 2*a(n-2) + 5*a(n-3) - 8*a(n-4) + 2*a(n-5)
 *          - 9*a(n-6) + 7*a(n-7) - a(n-8) + 8*a(n-9) + a(n-10)
 *          - 4*a(n-11) - 3*a(n-13) + 2*a(n-14)
 * Initial values:
 *   a(1)=16, a(2)=89, a(3)=100, a(4)=169, a(5)=311,
 *   a(6)=749, a(7)=1803, a(8)=4257, a(9)=10353,
 *   a(10)=25491, a(11)=62623, a(12)=154129,
 *   a(13)=380187, a(14)=938001, a(15)=2314659,
 *   a(16)=5713631, a(17)=14105377
 */

#include <stdlib.h>

/* The build system defines the integer types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[1..] stores computed values */
    static seq_t *cache   = NULL;
    static size_t  size   = 0;
    static idx_t   max_i  = 0;

    if (n < 1)                     /* no defined value for n < 1 */
        return (seq_t)0;

    /* First call: allocate cache and initialise the 17 known values */
    if (!cache) {
        size = 18;                         /* indices 0..17 */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache)                        /* allocation failure */
            return (seq_t)0;

        cache[0] = (seq_t)0;                /* dummy entry */

        cache[1]  = (seq_t)16;
        cache[2]  = (seq_t)89;
        cache[3]  = (seq_t)100;
        cache[4]  = (seq_t)169;
        cache[5]  = (seq_t)311;
        cache[6]  = (seq_t)749;
        cache[7]  = (seq_t)1803;
        cache[8]  = (seq_t)4257;
        cache[9]  = (seq_t)10353;
        cache[10] = (seq_t)25491;
        cache[11] = (seq_t)62623;
        cache[12] = (seq_t)154129;
        cache[13] = (seq_t)380187;
        cache[14] = (seq_t)938001;
        cache[15] = (seq_t)2314659;
        cache[16] = (seq_t)5713631;
        cache[17] = (seq_t)14105377;

        max_i = 17;
    }

    /* Grow the cache if it cannot hold the requested index */
    if (n >= size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)            /* realloc failed */
            return (seq_t)0;
        cache = tmp;
        size  = new_size;
    }

    /* Return the value if it has already been computed */
    if (n <= max_i)
        return cache[n];

    /* Compute the missing terms up to n using the recurrence */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        if (i <= 17) {   /* the first 17 terms are already stored */
            continue;
        }
        seq_t pos = 0;
        seq_t neg = 0;

        /* add the positive‑coefficient terms */
        pos += (seq_t)3 * cache[i-1];
        pos += (seq_t)5 * cache[i-3];
        pos += (seq_t)2 * cache[i-5];
        pos += (seq_t)7 * cache[i-7];
        pos += (seq_t)8 * cache[i-9];
        pos += (seq_t)1 * cache[i-10];
        pos += (seq_t)2 * cache[i-14];

        /* add the negative‑coefficient terms */
        neg += (seq_t)2 * cache[i-2];
        neg += (seq_t)8 * cache[i-4];
        neg += (seq_t)9 * cache[i-6];
        neg += (seq_t)1 * cache[i-8];
        neg += (seq_t)4 * cache[i-11];
        neg += (seq_t)3 * cache[i-13];

        /* result = positive sum – negative sum */
        cache[i] = pos - neg;
    }

    max_i = n;
    return cache[n];
}
