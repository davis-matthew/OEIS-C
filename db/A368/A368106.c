/*
 * A368106  The number of infinitary divisors of the powerful part of n.
 * https://oeis.org/A368106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A037445(A057521(n)) Multiplicative with a(p) = 1 and
 *            a(p^e) = 2^popcount(e) for e ≥ 2 (popcount(e) is the number of
 *            1‑bits in the binary representation of e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* Generate the prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;            /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e >= 2) {
            /* popcount of e using only integer operations */
            unsigned int bits = 0;
            uint64_t tmp = e;
            while (tmp) {
                bits += (unsigned int)(tmp & 1);
                tmp >>= 1;
            }
            result *= ((seq_t)1 << bits);
        }
    }
    return result;
}
