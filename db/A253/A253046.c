/*
 * A253046  An involution of the natural numbers: if n = 2*p_i then replace n
 *          with 3*p_{i+1}, and conversely if n = 3*p_i then replace n with
 *          2*p_{i-1}, where p_i denotes the i-th prime.
 * https://oeis.org/A253046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is not 2×prime or 3×prime; Otherwise if n = 2*p_i
 *            then a(n) = 3*p_{i+1}, if n = 3*p_i then a(n) = 2*p_{i-1} (p_0
 *            undefined)
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* First check for n = 2 * prime */
    if ((n % 2) == 0) {
        uint64_t m = n / 2;
        if (is_prime_big((seq_t)m)) {
            uint64_t nextp = next_prime(m);
            if (nextp != 0) {
                return (seq_t)3 * (seq_t)nextp;
            }
        }
    }

    /* Next check for n = 3 * prime */
    if ((n % 3) == 0) {
        uint64_t m = n / 3;
        if (is_prime_big((seq_t)m)) {
            uint64_t prevp = prev_prime(m);
            if (prevp != 0) {
                return (seq_t)2 * (seq_t)prevp;
            }
        }
    }

    /* Default: a(n) = n */
    return (seq_t)n;
}
