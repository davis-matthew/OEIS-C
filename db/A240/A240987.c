/*
 * A240987  (2^(p-1) modulo p^2) + (3^(p-1) modulo p^2), where p = prime(n).
 * https://oeis.org/A240987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(prime(n)-1) mod prime(n)^2) + (3^(prime(n)-1) mod
 *            prime(n)^2)
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 5003, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* kth prime (p = prime(n)) */
    uint32_t up = get_prime(n);
    uint64_t p = (uint64_t)up;

    seq_t mod = (seq_t)p * (seq_t)p;          /* p^2 */
    seq_t t2  = mod_pow((seq_t)2, p - 1, mod);
    seq_t t3  = mod_pow((seq_t)3, p - 1, mod);

    return t2 + t3;
}
