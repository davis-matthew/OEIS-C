/*
 * A331599  a(n) = A241909(n) / gcd(A122111(n),A241909(n)).
 * https://oeis.org/A331599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A241909(n) / gcd(A122111(n), A241909(n)). Both factors
 *            are built as exponent vectors over the primes, so a(n) =
 *            Product_j prime(j)^max(0, e_j - f_j), where e_j and f_j are the
 *            exponents of prime(j) in A241909(n) and A122111(n). The large
 *            values of A241909(n) and A122111(n) are never formed...
 * Range:     u8 n <= 33, u16 n <= 73, u32 n <= 157, u64 n <= 361,
 *            u128 n <= 841, big any n
 * Validated: standard, pass, 106 terms through n = 8864 (2026-11-08)
 * Helpers:   factor_u64, ipow_idx, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"

#define A331599_MAXK 130

static uint64_t A331599_prime(size_t k) /* k >= 1, k <= A331599_MAXK */
{
    static uint64_t pr[A331599_MAXK + 1];
    static size_t sz = 0;
    if (sz == 0) {
        uint64_t c = 2;
        while (sz < A331599_MAXK) {
            bool ok = true;
            for (size_t i = 0; i < sz; ++i) {
                if (pr[i] * pr[i] > c) break;
                if (c % pr[i] == 0) { ok = false; break; }
            }
            if (ok) pr[sz++] = c;
            ++c;
        }
    }
    return pr[k - 1];
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* sorted prime indices with multiplicity */
    idx_t ind[A331599_MAXK];
    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t pi = (idx_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) ind[k++] = pi;
    }

    idx_t e241909[A331599_MAXK + 2];
    idx_t e122111[A331599_MAXK + 2];
    for (size_t j = 0; j < A331599_MAXK + 2; ++j) { e241909[j] = 0; e122111[j] = 0; }

    /* A241909 exponents */
    for (size_t j = 0; j < k; ++j) {
        if (j == 0) e241909[1] += ind[0] - (idx_t)1;
        else        e241909[j + 1] += ind[j] - ind[j - 1];
    }
    e241909[k] += (idx_t)1;

    /* A122111 exponents (conjugate partition) */
    {
        idx_t remaining = (idx_t)k;
        idx_t last_part = 1;
        size_t pos = 0;
        while (pos < k) {
            idx_t cur = ind[pos];
            size_t freq = 0;
            while (pos < k && ind[pos] == cur) { ++freq; ++pos; }
            e122111[(size_t)remaining] += cur - last_part + (idx_t)1;
            remaining -= (idx_t)freq;
            last_part = cur + (idx_t)1;
        }
    }

    seq_t result = (seq_t)1;
    for (size_t j = 1; j <= k; ++j) {
        if (e241909[j] > e122111[j]) {
            idx_t d = e241909[j] - e122111[j];
            result *= ipow_idx((seq_t)A331599_prime(j), d);
        }
    }
    return result;
}
