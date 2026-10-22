/*
 * A332777  a(n) = k^2 mod p where p is the n-th prime and of the form 6k-1 or
 *          6k+1.
 * https://oeis.org/A332777
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)/6)^2 mod p if p≡1 (mod 6), else ((p+1)/6)^2 mod p,
 *            where p is the n‑th prime (n ≥ 3)
 * Range:     u8 n <= 65, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;  /* undefined below the offset */
    }
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;
    uint64_t k;
    if (p32 % 6u == 1u) {
        k = (p - 1) / 6;
    } else {
        k = (p + 1) / 6;
    }
    uint64_t r = (k * k) % p;
    return (seq_t)r;
}
