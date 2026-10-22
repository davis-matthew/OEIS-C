/*
 * A328621  Multiplicative with a(p^e) = p^(2e mod p).
 * https://oeis.org/A328621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^((2*e) mod p)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9641, u64 n <= 9641,
 *            u128 n <= 9641, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)1;
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t rem = ((e % p) * 2ULL) % p;     // (2*e) mod p
        seq_t val = big_pow((seq_t)p, (seq_t)rem);
        result *= val;
    }
    return result;
}
