/*
 * A398463  Multiplicative with a(p^e) = e^2 * p^e.
 * https://oeis.org/A398463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   a(n) = n * ∏_{p^e || n} e^2 (multiplicative, a(p^e) = e^2 * p^e)
 * Range:     u8 n <= 15, u16 n <= 575, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */

seq_t a(idx_t n) {
    if (n == 0)   /* undefined for OEIS offset 1, but guard anyway */
        return (seq_t)0;

    /* convert index to seq_t for computations */
    seq_t result = (seq_t)n;          /* will hold n * ∏ e^2 */

    if (n == 1)     /* a(1) = 1 * 1 = 1 */
        return result;       /* n==1, product over empty set = 1 */

    /* factorization */
    /* allocation for up to 64 distinct prime factors (enough for 64‑bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* multiply by each exponent squared */
    seq_t e2_product = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        /* e^2 fits in seq_t because e is small (<=64) */
        seq_t e = (seq_t)exps[i];
        e2_product *= e * e;
    }

    result *= e2_product;
    return result;
}
