/*
 * A334964  Numbers that are the sum of three coprime positive cubes.
 * https://oeis.org/A334964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6055, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_cube_root_u64, _gcd_seq, _is_sum_of_three_coprime_cubes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by: a(n) is the nth positive integer that can be
   written as i^3 + j^3 + k^3 with i, j, k ≥ 1 and gcd(i, j, k) = 1.
   The function below searches for such numbers and memoises them.       */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_cube_root_u64.h"
#include "../utils/_gcd_seq.h"
#include "../utils/_is_sum_of_three_coprime_cubes.h"

/* ----------  helper: GCD for seq_t  ---------- */

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* number of cached terms */
    static uint64_t checked_up_to = 0;  /* last number tested for property */

    if (n == 0) return 0;               /* offset starts at 1 */

    /* Expand the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        ++checked_up_to;
        if (_is_sum_of_three_coprime_cubes((seq_t)checked_up_to)) {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            /* realloc with a NULL pointer behaves like malloc */
            if (!tmp) {                 /* allocation failed – return zero */
                cache = NULL;
                cache_len = 0;
                checked_up_to = 0;
                return 0;
            }
            cache = tmp;
            cache[cache_len++] = (seq_t)checked_up_to;
        }
    }
    return cache[n - 1];
}
