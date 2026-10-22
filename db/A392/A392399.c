/*
 * A392399  a(n) = Sum_{k=0..floor(2*n/9)} binomial(2*n-7*k,2*k).
 * https://oeis.org/A392399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-5) + a(n-9) (n ≥ 9) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=4, a(6)=11,
 *            a(7)=22, a(8)=37
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 76, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   size  = 0; /* allocated array size */
    static idx_t   ready = 0; /* first index that still needs computing */

    /* One‑time initialization of the first nine terms */
    if (size == 0) {
        size = 9;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)11;
        cache[7] = (seq_t)22;
        cache[8] = (seq_t)37;

        ready = 9; /* indices < ready are already initialized */
    }

    /* Fast return for n < 9 */
    if (n < 9) {
        return cache[n];
    }

    /* Grow the array if necessary */
    if (n + 1 > size) {
        size_t new_size = size;
        while (new_size <= (size_t)n) {
            new_size <<= 1;                 /* double until sufficient */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failure */
        cache = tmp;
        size  = (idx_t)new_size;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = ready; i <= n; ++i) {
        seq_t val = cache[i-1] + cache[i-1];   /* 2*a(i-1) */
        val -= cache[i-2];                     /* - a(i-2) */
        val += cache[i-5] * (seq_t)4;          /* + 4*a(i-5) */
        val += cache[i-9];                     /* + a(i-9) */
        cache[i] = val;
    }

    ready = n + 1;
    return cache[n];
}
