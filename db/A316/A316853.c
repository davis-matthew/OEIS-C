/*
 * A316853  Areas of all nondegenerate integer triangles, T, expressed as
 *          16*area(T)^2. a(n) is for the triangle with sides A316841(n,
 *          1..3).
 * https://oeis.org/A316853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a+b+c) * (b+c-a) * (c+a-b) * (a+b-c) where the triangle
 *            sides are ordered by 1) increasing maximum side c, 2) increasing
 *            second side b, 3) increasing minimum side a. This is the
 *            ordering used by OEIS A316853
 * Range:     u8 n <= 8, u16 n <= 238, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* State for generating the next triangle */
    static idx_t cur_c = 1, cur_b = 1, cur_a = 0;

    /* Ensure we have at least n terms in the cache */
    while (cache_size < (size_t)n)
    {
        /* Minimal a for the current (b,c) that satisfies a + b > c */
        idx_t min_a = cur_c - cur_b + 1;
        if (min_a < 1)
            min_a = 1;

        /* Advance a to be at least min_a */
        if (cur_a < min_a)
            cur_a = min_a;

        /* If a exceeded b, move to the next b */
        if (cur_a > cur_b)
        {
            cur_b++;
            if (cur_b > cur_c)
            {
                cur_c++;
                cur_b = 1;
            }
            cur_a = 0;       /* reset a when b changes */
            continue;
        }

        /* Compute the value for the triangle (cur_a, cur_b, cur_c) */
        seq_t a_s = (seq_t)cur_a;
        seq_t b_s = (seq_t)cur_b;
        seq_t c_s = (seq_t)cur_c;

        seq_t sum = a_s + b_s + c_s;
        seq_t t1  = b_s + c_s - a_s;
        seq_t t2  = c_s + a_s - b_s;
        seq_t t3  = a_s + b_s - c_s;

        seq_t val = sum * t1;
        val = val * t2;
        val = val * t3;

        /* Grow the cache when necessary and store the value */
        if (cache_size >= cache_cap)
            grow_seq(&cache, &cache_cap, cache_size + 1);

        cache[cache_size++] = val;

        /* Prepare for the next triangle */
        cur_a++;
    }

    return cache[(size_t)n - 1];
}
