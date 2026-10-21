/*
 * A268307  Integers n such that A005897(n) is the sum of two positive cubes.
 * https://oeis.org/A268307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268307:
 *  Integers n such that A005897(n) = 6*n^2 + 2 is the sum of two positive cubes.
 *  The function returns the n-th such integer (offset 1) using incremental search.
 *  It is memoized, so calls in any order are efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

static bool is_sum_two_cubes(seq_t t)
{
    seq_t a_root = floor_cube_root(t);
    for (seq_t a = 1; a <= a_root; ++a) {
        seq_t a_cube = a * a * a;
        if (a_cube >= t)
            break;
        seq_t b_cube = t - a_cube;
        if (b_cube == 0)          /* b must be >= 1 */
            continue;
        seq_t b_root = floor_cube_root(b_cube);
        if (b_root * b_root * b_root == b_cube)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t current_n = 1;   /* next n to test */

    /* Grow the cache until it holds at least n entries */
    while (cache_size < n) {
        seq_t nn = (seq_t)current_n;
        seq_t t = (seq_t)6 * nn * nn + (seq_t)2;   /* A005897(nn) = 6*n^2 + 2 */

        if (is_sum_two_cubes(t)) {
            /* enlarge array if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();               /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = nn;
        }

        ++current_n;
    }

    return cache[n - 1];
}
