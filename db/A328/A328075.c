/*
 * A328075  Numbers such that the absolute values of the differences between
 *          any pair of digits are distinct.
 * https://oeis.org/A328075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 1862, u32 n <= 1862, u64 n <= 1862,
 *            u128 n <= 1862, big any n
 * Helpers:   distinct_digit_differences
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A328075: numbers whose pairwise absolute digit differences are all distinct.
   The function enumerates all positive integers up to five decimal digits
   (the maximum length that can satisfy the distinct‑difference condition),
   stores those that satisfy the property in a static array, and then
   returns the nth element of that array.  For indices out of range
   the function returns 0.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/distinct_digit_differences.h"

/* Static cache for all qualifying numbers (max 90 000 entries).  The array
   is only filled on the first call to a().  */
static seq_t  seq_cache[100000];
static size_t seq_len  = 0;
static bool   initialized = false;

/* Main OEIS function. */
seq_t a(idx_t n)
{
    /* Build the cache once. */
    if (!initialized) {
        const unsigned int MAX_NUM = 99999U; /* 5‑digit upper bound */
        seq_len = 0;

        for (unsigned int m = 10U; m <= MAX_NUM; ++m) {
            if (distinct_digit_differences(m)) {
                seq_cache[seq_len++] = (seq_t)m;
            }
        }
        initialized = true;
    }

    /* Validate index. */
    if (n == 0 || (size_t)(n - 1) >= seq_len) {
        return (seq_t)0;                  /* default‑constructed value */
    }
    return seq_cache[n - 1];
}
