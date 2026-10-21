/*
 * A271099  Number of ordered ways to write n as u^3 + v^3 + 2*x^3 + 2*y^3 +
 *          3*z^3, where u, v, x, y and z are nonnegative integers with u <= v
 *          and x <= y.
 * https://oeis.org/A271099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of tuples (u,v,x,y,z) of non‑negative integers
   with u <= v, x <= y such that u³ + v³ + 2·x³ + 2·y³ + 3·z³ = n.  */

#include <stdlib.h>

/* Integer cube root: largest k with k³ <= n.  Uses __int128 to avoid overflow. */
static inline idx_t icbrt(idx_t n)
{
    idx_t k = 0;
    while (1) {
        idx_t next = k + 1;
        unsigned __int128 cube = (unsigned __int128)next * next * next;
        if (cube > n)
            break;
        k = next;
    }
    return k;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[0 .. cache_cap-1] */
    static idx_t cache_cap = 0;

    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;                 /* allocation failure */

        /* initialise */
        for (idx_t i = 0; i < new_cap; ++i)
            new_cache[i] = 0;

        /* Enumerate all tuples and fill the cache */
        for (idx_t u = 0; u <= icbrt(new_cap - 1); ++u) {
            unsigned __int128 u3_128 = (unsigned __int128)u * u * u;
            if (u3_128 > new_cap - 1)
                break;
            idx_t u3 = (idx_t)u3_128;
            idx_t rem1 = new_cap - 1 - u3;

            for (idx_t v = u; v <= icbrt(rem1); ++v) {
                unsigned __int128 v3_128 = (unsigned __int128)v * v * v;
                if (v3_128 > rem1)
                    break;
                idx_t v3 = (idx_t)v3_128;
                idx_t sum_uv = u3 + v3;
                if (sum_uv > new_cap - 1)
                    break;
                idx_t rem2 = new_cap - 1 - sum_uv;

                for (idx_t x = 0; x <= icbrt(rem2 / 2); ++x) {
                    unsigned __int128 x3_128 = (unsigned __int128)x * x * x;
                    idx_t x3 = (idx_t)x3_128;
                    idx_t part_x = 2 * x3;
                    if (part_x > rem2)
                        break;
                    idx_t rem3 = rem2 - part_x;

                    for (idx_t y = x; y <= icbrt(rem3 / 2); ++y) {
                        unsigned __int128 y3_128 = (unsigned __int128)y * y * y;
                        idx_t y3 = (idx_t)y3_128;
                        idx_t part_xy = part_x + 2 * y3;
                        if (part_xy > rem2)
                            break;
                        idx_t rem4 = rem2 - part_xy;

                        for (idx_t z = 0; z <= icbrt(rem4 / 3); ++z) {
                            unsigned __int128 z3_128 = (unsigned __int128)z * z * z;
                            idx_t z3 = (idx_t)z3_128;
                            idx_t total = sum_uv + part_xy + 3 * z3;

                            if (total < new_cap)
                                ++new_cache[total];
                            else
                                break;
                        }
                    }
                }
            }
        }

        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[n];
}
