/*
 * A343750  Let S be the set of all numbers that can be obtained by permuting
 *          the digits of n (leading zeros can be omitted). Then a(n) is that
 *          element of S with the smallest number of divisors. In case of a
 *          tie, choose the smallest.
 * https://oeis.org/A343750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = the smallest permutation of the digits of n that has the
 * fewest divisors.  If several permutations have the same minimal
 * divisor count, the numerically smallest such permutation is chosen.
 * The implementation brute‑forces all unique permutations of the
 * decimal digits of n.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/divisor_count_u64.h"

/* Helper: swap two uint8_t values */
static void swap_uint8(uint8_t *a, uint8_t *b)
{
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Recursive generation of all unique permutations of digits[].
 * len  : total number of digits
 * pos  : current position to fix
 * best : pointer to current best value (seq_t)
 * bestDiv : pointer to divisor count of current best (uint64_t)
 */
static void permute_unique(uint8_t *digits, uint8_t len, uint8_t pos,
                           seq_t *best, uint64_t *bestDiv)
{
    if (pos == len) {
        /* compute numeric value of current permutation */
        seq_t val = 0;
        for (uint8_t i = 0; i < len; ++i)
            val = val * 10 + digits[i];

        uint64_t d = divisor_count_u64((uint64_t)val);
        if (d < *bestDiv || (d == *bestDiv && val < *best)) {
            *bestDiv = d;
            *best = val;
        }
        return;
    }

    for (uint8_t i = pos; i < len; ++i) {
        /* skip duplicates: if digits[i] has already appeared
           at an earlier position in this recursion level, skip */
        bool dup = false;
        for (uint8_t j = pos; j < i; ++j)
            if (digits[j] == digits[i]) { dup = true; break; }
        if (dup) continue;

        swap_uint8(&digits[pos], &digits[i]);
        permute_unique(digits, len, pos + 1, best, bestDiv);
        swap_uint8(&digits[pos], &digits[i]);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* behavior for n=0 is irrelevant */

    /* Extract digits of n into an array */
    uint8_t digits[20];
    idx_t tmp = n;
    uint8_t len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    /* reverse to get most‑significant digit first */
    for (uint8_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t best = 0;
    uint64_t bestDiv = UINT_MAX;  /* UINT_MAX from <limits.h> is safe */
    permute_unique(digits, len, 0, &best, &bestDiv);
    return best;
}
