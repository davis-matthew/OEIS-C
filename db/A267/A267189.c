/*
 * A267189  (Cubes of positive numbers) that are not the sum of three nonzero
 *          squares.
 * https://oeis.org/A267189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m³ where m is the n‑th positive integer that is * either
 *            a power of two, or of the form m = 2^(2k) * t with t odd * and t
 *            ≡ 7 (mod 8). The numbers m are taken in increasing order
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 279, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t last_candidate = 1;
    static idx_t max_index = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    while (max_index < n)
    {
        const idx_t candidate = last_candidate;
        idx_t m = candidate;
        idx_t e = 0;
        /* remove factors of 2 */
        while ((m & 1) == 0)
        {
            m >>= 1;
            ++e;
        }
        /* accept if m==1 (power of 2) or (e even and m ≡ 7 (mod 8)) */
        if (m == 1 || ((e & 1) == 0 && (m % 8) == 7))
        {
            /* compute m³ in seq_t */
            seq_t cube = (seq_t)candidate * candidate * candidate;

            if (max_index == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
            }
            cache[max_index++] = cube;
        }
        ++last_candidate;
    }
    return cache[n - 1];
}
