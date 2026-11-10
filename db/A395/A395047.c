/*
 * A395047  a(n) = A241917(A003415(n)).
 * https://oeis.org/A395047
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A241917(A003415(n)), with A003415(n) = n*Sum_{p^e || n}
 *            e/p and A241917(1) = 0, A241917(p_i) = i, else index difference
 *            of the two largest prime factors (0 if the largest repeats)
 * Range:     u8 n <= 1721, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9263 (2026-11-10)
 * Helpers:   factor_u64, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"

static uint64_t A003415_d(uint64_t n)
{
    if (n < 2)
        return 0;
    uint64_t result = 0;
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        uint64_t e = 0;
        while (rem % p == 0) {
            e++;
            rem /= p;
        }
        if (e > 0)
            result += (n / p) * e;
    }
    if (rem > 1)
        result += n / rem;
    return result;
}

static seq_t A241917_a(idx_t n)
{
    if (n <= 1)
        return 0;
    if (is_prime(n))
        return (seq_t)prime_pi(n);

    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt = 0;
    factor_u64(n, primes_f, exps_f, &cnt);

    idx_t max_idx = 0;
    size_t max_exp = 0;
    idx_t second_idx = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes_f[i];
        size_t ex = (size_t)exps_f[i];
        idx_t idx = (idx_t)prime_pi(p);
        if (idx > max_idx) {
            second_idx = max_idx;
            max_idx = idx;
            max_exp = ex;
        } else if (idx == max_idx) {
            max_exp += ex;
        } else if (idx > second_idx) {
            second_idx = idx;
        }
    }

    if (max_exp >= 2)
        return 0;
    if (second_idx == 0)
        return 0;
    return (seq_t)(max_idx - second_idx);
}

seq_t a(idx_t n)
{
    return A241917_a((idx_t)A003415_d((uint64_t)n));
}
