/*
 * A373246  Greatest common divisor of n and its prime shadow: a(n) = gcd(n,
 *          A181819(n)).
 * https://oeis.org/A373246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A181819(n)) = prod over primes q of q^min(v_q(n),
 *            #{i : e_i = pi(q)}), where n = prod p_i^e_i
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9964 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static uint64_t A373246_kth_prime(uint64_t k)
{
    uint64_t c = 1;
    uint64_t cnt = 0;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    seq_t result = (seq_t)1;

    if (n <= 1) return result;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        bool seen = false;
        for (size_t j = 0; j < i; ++j) {
            if (ex[j] == ex[i]) { seen = true; break; }
        }
        if (seen) continue;

        uint64_t c = 0;
        for (size_t j = 0; j < cnt; ++j) {
            if (ex[j] == ex[i]) c++;
        }
        uint64_t q = A373246_kth_prime(ex[i]);

        for (size_t j = 0; j < cnt; ++j) {
            if (pr[j] == q) {
                uint64_t m = ex[j] < c ? ex[j] : c;
                for (uint64_t t = 0; t < m; ++t) result = result * (seq_t)q;
                break;
            }
        }
    }
    return result;
}
