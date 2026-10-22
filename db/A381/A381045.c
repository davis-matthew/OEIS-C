/*
 * A381045  Happy cubes: cubes whose trajectory under iteration of sum of
 *          squares of digits map includes 1.
 * https://oeis.org/A381045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 217, u64 n <= 1004,
 *            u128 n <= 1004, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A381045: a(n) is the n‑th cube of an integer whose cube
   is a happy number.  An integer N is happy if repeated application
   of the sum‑of‑squares‑of‑digits map eventually reaches 1.  This
   implementation tests each cube m³ for happiness and caches the
   results.  The sequence is generated in order of increasing cube
   root m, not by the numerical value of the cube.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"

static inline seq_t digit_square_sum_seqt(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    /* Sum of squared digits computed from the digit counts */
    return (seq_t)(cnt[0] * 0  +
                  cnt[1] * 1  +
                  cnt[2] * 4  +
                  cnt[3] * 9  +
                  cnt[4] * 16 +
                  cnt[5] * 25 +
                  cnt[6] * 36 +
                  cnt[7] * 49 +
                  cnt[8] * 64 +
                  cnt[9] * 81);
}

static inline bool is_happy(seq_t n)
{
    /* The unhappy cycle always passes through 4, so we can stop there. */
    while (n != 1 && n != 4) {
        n = (seq_t)digit_square_sum_seqt(n);
    }
    return n == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; value for n=0 is undefined */

    static seq_t *cache = NULL;      /* cached happy cubes */
    static size_t cache_cap = 0;     /* capacity of cache */
    static size_t cache_len = 0;     /* number of cached values */
    static idx_t next = 1;           /* next integer to test for a cube root */

    while (cache_len < n) {
        idx_t m  = next++;         /* cube root candidate */
        /* Compute m³ */
        seq_t mk = (seq_t)m;
        seq_t mk2 = mk * mk;       /* m² */
        seq_t cube = mk2 * mk;     /* m³ */

        if (is_happy(cube)) {
            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (cache) {
                    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cube;
        }
    }
    return cache[n - 1];
}
