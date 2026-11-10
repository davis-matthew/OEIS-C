/*
 * A280489  a(n) = gcd(n,A241909(n)).
 * https://oeis.org/A280489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A241909(n)), computed prime by prime: A241909(n) =
 *            prod_j p_j^e_j with e_1 = i_1-1, e_j = i_j-i_{j-1}, plus 1 on
 *            the last exponent; a(n) = prod_j p_j^min(e_j, v_{p_j}(n))
 * Range:     u8 n <= 1099, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 135 terms through n = 7112 (2026-11-08)
 * Helpers:   factor_u64, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    if (n <= (idx_t)1) return result;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    uint64_t indices[128];
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pi = (uint64_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) {
            indices[total++] = pi;
        }
    }

    for (size_t j = 0; j < total; ++j) {
        uint64_t e;
        if (j == 0) e = indices[0] - 1;
        else e = indices[j] - indices[j - 1];
        if (j == total - 1) e += 1;
        if (e == 0) continue;

        uint64_t p = (uint64_t)nth_prime((idx_t)(j + 1));
        uint64_t m = (uint64_t)n;
        uint64_t v = 0;
        while (m % p == 0) { m /= p; ++v; }
        uint64_t t = (v < e) ? v : e;
        for (uint64_t q = 0; q < t; ++q) result *= (seq_t)p;
    }
    return result;
}
