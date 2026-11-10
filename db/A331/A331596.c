/*
 * A331596  Number of distinct prime factors of gcd(A122111(n), A241909(n)).
 * https://oeis.org/A331596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(gcd(A122111(n), A241909(n))), computed from the
 *            prime-index exponent vectors of both values, so there is no
 *            overflow
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9130 (2026-11-08)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    idx_t ind[130];
    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t pi = (idx_t)prime_pi(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) ind[k++] = pi;
    }

    bool s1[131], s2[131];
    for (size_t i = 0; i <= k; ++i) { s1[i] = false; s2[i] = false; }

    /* A122111: prime index r (remaining count) appears for each group */
    {
        size_t remaining = k, pos = 0;
        while (pos < k) {
            idx_t cur = ind[pos];
            size_t freq = 0;
            while (pos < k && ind[pos] == cur) { ++freq; ++pos; }
            s1[remaining] = true;
            remaining -= freq;
        }
    }

    /* A241909: prime p_j appears when its exponent is positive */
    if (k == 1) {
        s2[1] = true;  /* a(p_i) = 2^i, i >= 1 */
    } else {
        for (size_t j = 0; j < k; ++j) {
            idx_t e;
            if (j == 0) e = ind[0] - 1;
            else if (j == k - 1) e = 1 + ind[j] - ind[j - 1];
            else e = ind[j] - ind[j - 1];
            if (e > 0) s2[j + 1] = true;
        }
    }

    seq_t c = 0;
    for (size_t i = 1; i <= k; ++i)
        if (s1[i] && s2[i]) ++c;
    return c;
}
