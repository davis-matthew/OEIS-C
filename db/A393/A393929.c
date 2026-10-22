/*
 * A393929  a(n) is the sum of divisors of n whose decimal representation is
 *          not a substring of the decimal representation of n.
 * https://oeis.org/A393929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = Sum_{d|n} d * [str(d) is not a substring of str(n)]
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   not_substring
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all divisors of n, checks the
   substring condition, and accumulates the sum. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/not_substring.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the sequence, return 0 */
        return 0;

    seq_t sum = 0;
    char n_str[22];
    snprintf(n_str, sizeof n_str, "%llu", (unsigned long long)n);

    idx_t limit = (idx_t)floor(sqrt((double)n));
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (not_substring(n_str, d))
                sum += (seq_t)d;

            idx_t comp = n / d;
            if (comp != d) { /* complementary divisor */
                if (not_substring(n_str, comp))
                    sum += (seq_t)comp;
            }
        }
    }
    return sum;
}
