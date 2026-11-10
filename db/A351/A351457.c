/*
 * A351457  a(n) = A351456(n) - A339905(n).
 * https://oeis.org/A351457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A351456(n) - A339905(n), where A351456(n) =
 *            A003958(sigma(A003961(n))) and A339905 is completely
 *            multiplicative with a(prime(k)) = prime(k+1) - 1
 * Range:     i8 n <= 10, i16 n <= 1023, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 8383 (2026-11-10)
 * Helpers:   factor_u64, ipow_u64_idx, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t m = 1;       /* A003961(n) */
    seq_t b = 1;       /* A339905(n) */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);
        m *= ipow_u64_idx(q, (idx_t)exps[i]);
        b *= ipow_u64_idx(q - 1, (idx_t)exps[i]);
    }

    factor_u64((uint64_t)m, primes, exps, &cnt);
    seq_t sig = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t r = primes[i];
        seq_t r_pow = ipow_u64_idx(r, (idx_t)(exps[i] + 1));
        sig *= (r_pow - 1) / (seq_t)(r - 1);
    }

    factor_u64((uint64_t)sig, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        res *= ipow_u64_idx(primes[i] - 1, (idx_t)exps[i]);
    }

    return res - b;
}
