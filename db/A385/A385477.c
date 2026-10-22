/*
 * A385477  Composite numbers whose digits are odd prime numbers.
 * https://oeis.org/A385477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 181, u32 n <= 673, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385477: composite numbers composed solely of the odd prime digits 3, 5, 7,
   listed in increasing order.  The algorithm generates all numbers with digits
   {3,5,7} in numeric order and selects the composite ones. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Recursive helper that builds numbers of a fixed length.  It stops early
   when the target index has been reached. */
static bool dfs_generate(int len, int pos, seq_t val,
                         idx_t *cnt, idx_t target, seq_t *result)
{
    if (pos == len) {
        if (val > 1 && !is_prime_big(val)) {
            ++(*cnt);
            if (*cnt == target) {
                *result = val;
                return true;
            }
        }
        return false;
    }
    /* digits are tried in ascending order: 3, 5, 7 */
    for (int d = 3; d <= 7; d += 2) {
        if (dfs_generate(len, pos + 1, val * 10 + d,
                         cnt, target, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t result = 0;

    /* start with the smallest composite length (2 digits) */
    for (int len = 2; ; ++len) {
        if (dfs_generate(len, 0, 0, &count, n, &result))
            return result;
    }
}
