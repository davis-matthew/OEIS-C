/*
 * A244763  Prime numbers ending in the prime number 13.
 * https://oeis.org/A244763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime of the form 100*k + 13 (primes ≡ 13 (mod 100))
 * Range:     u8 n <= 2, u16 n <= 144, u32 n <= 145, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate candidates 13, 113, 213, … and count primes until the n‑th is found */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t cand = (seq_t)13;
    idx_t count = 0;
    while (count < n) {
        if (is_prime_big(cand)) {
            count++;
            if (count == n)
                break;
        }
        cand += (seq_t)100;
    }
    return cand;
}
