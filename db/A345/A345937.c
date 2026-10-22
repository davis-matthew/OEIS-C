/*
 * A345937  a(n) = gcd(n-1, uphi(n)), where uphi is unitary totient (or
 *          unitary phi) function, A047994.
 * https://oeis.org/A345937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n-1, uphi(n)) * where uphi(n) = product_{p^a||n} (p^a
 *            - 1)
 * Range:     u8 n <= 256, u16 n <= 9807, u32 n <= 9807, u64 n <= 9807,
 *            u128 n <= 9807, big any n
 * Helpers:   factor_u64, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* n-1 as seq_t */
    seq_t n_minus_one = (n == 0 ? 0 : (seq_t)n - 1U);

    /* factor n into prime powers */
    /* worst case number of distinct primes < log2(n) < 64 for any 64‑bit n */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute uphi(n) = product (p^a - 1) */
    seq_t uphi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* p^a as seq_t */
        seq_t pa = pow_seq((seq_t)primes[i], (int)exps[i]);
        /* (p^a - 1) */
        pa -= 1;
        uphi *= pa;
    }

    return gcd(uphi, n_minus_one);
}
