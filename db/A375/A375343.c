/*
 * A375343  Numbers which are the sixth powers of their digit sum.
 * https://oeis.org/A375343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th positive integer k such that digit_sum(k)^6 = k
 *            (OEIS A375343)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Provided utilities */
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"

/* Cached list of all solutions */
static seq_t solutions[256];
static size_t solution_count = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        /* Find all numbers k = s^6 with digit sum s up to the level where s^6
           can be at most 13 decimal digits (s <= 150 guarantees this).       */
        for (idx_t s = 0; s <= 150; ++s) {
            seq_t val = big_pow((seq_t)s, 6);
            seq_t sum = digit_sum(val);
            if (sum == (seq_t)s) {
                solutions[solution_count++] = val;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > solution_count) {
        /* Out-of-range: return 0 (sequence terminates after the known terms) */
        return 0;
    }
    return solutions[n - 1];
}
