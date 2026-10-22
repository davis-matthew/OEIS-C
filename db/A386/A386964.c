/*
 * A386964  a(1) = prime(1) = 2, a(n) = 10*a(n-1) + (prime(n) mod 10).
 * https://oeis.org/A386964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2, a(n) = 10*a(n-1) + (prime(n) mod 10)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);        /* fetch the i‑th prime */
        res = res * (seq_t)10 + (seq_t)(p % 10);
    }
    return res;
}
