/*
 * A352942  Let p = prime(n); a(n) = number of primes q with same number of
 *          binary digits as p that can be obtained from p by changing one
 *          binary digit.
 * https://oeis.org/A352942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of primes q that have the same binary digit *
 *            length as prime(n) and differ from it in exactly one bit
 * Range:     u8 n <= 55, u16 n <= 7158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0 */
    /* retrieve the n-th prime */
    uint32_t p = get_prime(n);
    /* number of bits in p */
    unsigned int L = bitlen_seq_t((seq_t)p);
    seq_t count = 0;

    /* flip each non‑leading bit and test */
    for (unsigned int k = 0; k < L - 1; ++k) {
        uint32_t q = p ^ (1u << k);          /* flip bit k */
        /* ensure q still has L bits (leading bit stays 1) */
        if (q >= (1u << (L - 1)) && is_prime((idx_t)q))
            ++count;
    }
    return count;
}
