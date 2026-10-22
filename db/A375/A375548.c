/*
 * A375548  a(n) is the least prime number of the form n mod (10^k) for some k
 *          > 0, or 0 if no such prime number exists.
 * https://oeis.org/A375548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least prime p such that p = n mod 10^k for some k>0; 0 if
 *            none
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* n = 0 or 1 gives no prime suffix */

    seq_t min_prime = 0;          /* 0 means no prime found yet */
    idx_t power = 10;             /* 10^1 */

    /* Check suffixes corresponding to 10, 100, 1000, ... up to power <= n */
    while (power <= n) {
        idx_t r = n % power;
        if (r > 1 && is_prime_big((seq_t)r)) {
            if (min_prime == 0 || (seq_t)r < min_prime)
                min_prime = (seq_t)r;
        }
        /* Avoid overflow of power * 10; stop if next would exceed n */
        if (power > n / 10) break;
        power *= 10;
    }

    /* Also consider the full number itself (k > number of digits) */
    if (n > 1 && is_prime_big((seq_t)n)) {
        if (min_prime == 0 || (seq_t)n < min_prime)
            min_prime = (seq_t)n;
    }

    return min_prime;
}
