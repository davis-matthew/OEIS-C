/*
 * A318476  Multiplicative with a(p^e) = 2^A000108(e).
 * https://oeis.org/A318476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^(sum_{p^e∣∣n} Catalan(e))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 31, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow, catalan, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/catalan.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t cat = catalan((idx_t)exps[i]);
        sum += cat;
    }
    return big_pow((seq_t)2, sum);
}
