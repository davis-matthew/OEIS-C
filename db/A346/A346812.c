/*
 * A346812  Positive numbers whose square starts with exactly 2 identical
 *          digits.
 * https://oeis.org/A346812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer m such that the decimal
 *            representation of m² starts with exactly two equal digits (and
 *            the third digit, if present, is different)
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? (*cap << 1) : 16;
        if (new_cap < needed) new_cap = needed;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined only for n >= 1 */

    /* Static state for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t m = next_candidate;
        seq_t sq = m * m;               /* m² */

        /* Count decimal digits of sq. */
        size_t D = 0;
        seq_t tmp = sq;
        while (tmp > 0) {
            tmp /= (seq_t)10;
            ++D;
        }

        if (D >= 2) {
            /* Compute 10^(D-2). */
            seq_t pow_two = 1;
            for (size_t i = 0; i < D - 2; ++i)
                pow_two *= (seq_t)10;

            seq_t t2 = sq / pow_two;           /* first two digits as a number */
            unsigned int first  = (unsigned int)(t2 / (seq_t)10);
            unsigned int second = (unsigned int)(t2 % (seq_t)10);

            if (first == second) {
                int third_ok = 1;               /* assume ok until proven otherwise */

                if (D >= 3) {
                    /* Compute 10^(D-3). */
                    seq_t pow_three = 1;
                    for (size_t i = 0; i < D - 3; ++i)
                        pow_three *= (seq_t)10;

                    unsigned int third = (unsigned int)((sq / pow_three) % (seq_t)10);
                    if (third == first)
                        third_ok = 0;
                }

                if (third_ok) {
                    grow_array(&cache, &cache_cap, cache_len + 1);
                    cache[cache_len++] = m;
                }
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
