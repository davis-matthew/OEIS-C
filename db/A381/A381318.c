/*
 * A381318  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (5^11*s+36643009) mod 2^30 when
 *          started at s=1.
 * https://oeis.org/A381318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^11 * a(n-1) + 36643009) mod 2^30, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* lazy initialization of cache */
    static seq_t *cache = NULL;
    static idx_t  count = 0;          /* number of stored terms (a(1) is cache[0]) */
    static const seq_t MULT = (seq_t)48828125;   /* 5^11 */
    static const seq_t INC  = (seq_t)36643009;
    static const uint64_t MASK = (1ULL << 30) - 1;  /* 2^30 - 1 */

    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 for safety */

    if (count == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(1) */
        count = 1;
    }

    while (count < n) {
        /* compute next state */
        uint64_t prev = (uint64_t)cache[count - 1];
        uint64_t next = (prev * (uint64_t)MULT + (uint64_t)INC) & MASK;

        /* resize cache */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(count + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[count] = (seq_t)next;
        ++count;
    }

    return cache[n - 1];
}
