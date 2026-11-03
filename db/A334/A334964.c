/*
 * A334964  Numbers that are the sum of three coprime positive cubes.
 * https://oeis.org/A334964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6055, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9876 (2026-11-03)
 * Helpers:   gcd, icbrt_u64, mem
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
#include "../utils/gcd.h"
#include "../utils/icbrt_u64.h"
#include "../utils/mem.h"

/* ----------  test if n is a sum of three coprime positive cubes  ---------- */
static bool _is_sum_of_three_coprime_cubes(seq_t n)
{
    if (n < 3) return false;                     /* 1^3 + 1^3 + 1^3 = 3 */

    uint64_t max_i = icbrt_u64((uint64_t)n - 2);
    for (uint64_t i = 1; i <= max_i; ++i) {
        uint64_t icub = i * i * i;
        uint64_t rem1 = (uint64_t)n - icub;

        uint64_t max_j = icbrt_u64(rem1 - 1);
        for (uint64_t j = 1; j <= max_j; ++j) {
            uint64_t jcub = j * j * j;
            uint64_t rem2 = rem1 - jcub;

            uint64_t k = icbrt_u64(rem2);
            unsigned __int128 kcub = (unsigned __int128)k * k * k;
            if (kcub == rem2 && k >= 1) {
                seq_t g = gcd((seq_t)i, (seq_t)j);
                g = gcd(g, (seq_t)k);
                if (g == 1) return true;
            }
        }
    }
    return false;
}

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
            seq_t *tmp = mem_resize(seq_t, cache, (cache_len + 1));
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
