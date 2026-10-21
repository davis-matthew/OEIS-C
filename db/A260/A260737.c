/*
 * A260737  Sum of Hamming distances between binary representations of prime
 *          factors of n, summed over all nonordered pairs of primes present
 *          (with multiplicity) in the prime factorization of n.
 * https://oeis.org/A260737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i<j} HammingDistance(p_i , p_j) where the p_i are the
 *            prime factors of n with multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bits_xor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/bits_xor.h"

static uint64_t _pf[64];   /* buffer for prime factors of the current n */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = (uint64_t)n;
    size_t cnt = 0;

    /* trial division, generating primes on the fly (2,3,5,…) */
    for (uint64_t p = 2;;) {
        if (p > temp / p)
            break;
        if (temp % p == 0) {
            do {
                _pf[cnt++] = p;
                temp /= p;
            } while (temp % p == 0);
        }
        p += (p == 2 ? 1 : 2);
        if (cnt >= sizeof(_pf) / sizeof(_pf[0]))
            break;   /* safety, should not happen for n ≤ 10 000 */
    }
    if (temp > 1)      /* remaining prime factor */
        _pf[cnt++] = temp;

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            unsigned int d = bits_xor(_pf[i], _pf[j]);
            result += (seq_t)d;
        }
    }
    return result;
}
