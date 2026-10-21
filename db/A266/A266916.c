/*
 * A266916  Multiply and add: distinct numbers a*b + c*d + e*f + g*h + i*j
 *          where a..j are permutations of 0..9.
 * https://oeis.org/A266916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   generate_permutations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A266916.
   The sequence consists of all distinct sums a*b + c*d + e*f + g*h + i*j
   where a…j are a permutation of 0..9.  The sums lie between 60 and 140
   and all values in this range appear except 135, 138, 139.  The values
   are returned in ascending order. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/generate_permutations.h"

static bool initialized = false;
static seq_t cache[81];     /* maximum possible number of distinct sums (81) */
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        bool used[10] = {0};
        int perm[10];
        bool seen[81] = {0};

        generate_permutations(0, used, perm, seen);

        size_t pos = 0;
        for (int sum = 60; sum <= 140; ++sum) {
            if (seen[sum - 60]) {
                cache[pos++] = (seq_t)sum;
            }
        }
        cache_len = pos;
        initialized = true;
    }

    if (n == 0 || (idx_t)n > (idx_t)cache_len)
        return (seq_t)0;   /* undefined for out of range indices */

    return cache[(size_t)(n - 1)];
}
