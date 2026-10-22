/*
 * A330315  a(n) = r(n)*r(n+1), where r(n) = A004018(n) is the number of ways
 *          of writing n as a sum of two squares.
 * https://oeis.org/A330315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n) * r(n+1) where r(n) = A004018(n) is the number of
 *            representations of n as a sum of two squares (ordered with
 *            signs). r(0) is defined to be 1
 * Range:     u8 n <= 984, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t r_value(idx_t n) {
    /* Returns the number of representations of n as a sum of two squares. */
    if (n == 0) return (seq_t)1;              /* r(0) = 1  (0 = 0²+0²) */

    uint64_t nn = (uint64_t)n;
    /* Factor n using the supplied factor_u64 routine. */
    /* 64 factors is plenty for any 64‑bit integer (max prime factors < 64). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* If any prime ≡ 3 (mod 4) occurs with odd exponent, no representations. */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 4 == 3 && (exps[i] & 1ULL))
            return (seq_t)0;
    }

    /* r(n) = 4 × ∏ (e_i + 1) over primes ≡ 1 (mod 4). */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 4 == 1)
            prod *= (seq_t)(exps[i] + 1);
    }
    prod *= (seq_t)4;
    return prod;
}

seq_t a(idx_t n) {
    seq_t r_n   = r_value(n);
    seq_t r_np1 = r_value(n + 1ULL);
    return r_n * r_np1;
}
