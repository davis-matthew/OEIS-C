/*
 * A319985  Fully multiplicative with a(p^e) = prime(p mod 12)^e.
 * https://oeis.org/A319985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over p^e || n of prime(p mod 12)^e * where
 *            prime(k) is the k-th prime number (prime(1)=2, prime(2)=3, *
 *            prime(3)=5, …, prime(11)=31)
 * Range:     u8 n <= 39, u16 n <= 1279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* handle n >= 1 */
    uint64_t nn = (uint64_t)n;

    /* factor the integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* mapping from residue mod 12 to the k-th prime */
    static const seq_t map[12] = {0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t r = p % 12ULL;
        seq_t base   = map[r];
        seq_t power  = seq_pow(base, e);
        result = result * power;
    }

    return result;
}
