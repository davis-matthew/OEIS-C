/*
 * A309772  Least common multiple of prime(n+1)+1 and prime(n)+1.
 * https://oeis.org/A309772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lcm(prime(n+1)+1, prime(n)+1)
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1; undefined for n<1 */
    uint32_t p_n = get_prime(n);          /* prime(n)   */
    uint32_t p_np1 = get_prime(n + 1);    /* prime(n+1) */
    seq_t val1 = (seq_t)p_n + 1;
    seq_t val2 = (seq_t)p_np1 + 1;
    seq_t g = gcd(val1, val2);
    return val1 / g * val2;
}
