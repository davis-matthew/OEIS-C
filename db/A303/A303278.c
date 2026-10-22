/*
 * A303278  If n = Product_j p_j^k_j where the p_j are distinct primes then
 *          a(n) = (Product_j k_j)^(Product_j p_j).
 * https://oeis.org/A303278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n/rad(n))^rad(n) (same as (∏k_j)^(∏p_j) for n =
 *            ∏p_j^k_j)
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 67, u64 n <= 131,
 *            u128 n <= 259, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    if (m <= 1) {
        return 1;               /* a(1) = 1 */
    }

    /* Factor n: obtain prime factors in `primes` and their exponents in `exps`. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t rad = 1;      /* product of distinct primes */
    seq_t prod_k = 1;   /* product of exponents */

    for (size_t i = 0; i < cnt; ++i) {
        rad     *= (seq_t)primes[i];
        prod_k  *= (seq_t)exps[i];
    }

    return big_pow(prod_k, rad);
}
