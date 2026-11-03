/*
 * A351613  Number of pairs of divisors of n, (d1,d2) d1<=d2, such that d1+d2
 *          is prime.
 * https://oeis.org/A351613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of divisors (d1,d2) with d1 ≤ d2 of n
 *            such that d1 + d2 is prime
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* Enumerate all divisors of n */
    idx_t divs[2048];
    size_t len = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[len++] = i;
            if (i != n / i) {
                divs[len++] = n / i;
            }
        }
    }

    seq_t count = 0;
    for (size_t a = 0; a < len; ++a) {
        for (size_t b = a; b < len; ++b) {
            seq_t sum = (seq_t)divs[a] + (seq_t)divs[b];
            if (is_prime_seq(sum)) {
                ++count;
            }
        }
    }
    return count;
}
