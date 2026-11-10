/*
 * A331731  Odd part of A331595(n), where A331595(n) = gcd(A122111(n),
 *          A241909(n)).
 * https://oeis.org/A331731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265(gcd(A122111(n), A241909(n))), computed on
 *            prime-exponent vectors: a(n) = Product_{j>=2} prime(j)^min(e1_j,
 *            e2_j), where e1 and e2 are the exponent vectors of A122111(n)
 *            and A241909(n)
 * Range:     u8 n <= 241, u16 n <= 1681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9446 (2026-11-08)
 * Helpers:   factor_u64, ipow_idx, nth_prime, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi_u64.h"
#include "../utils/nth_prime.h"

#define A331731_MAXK 130

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* prime indices with multiplicity, nondecreasing */
    uint64_t ind[A331731_MAXK];
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pi = prime_pi_u64(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) ind[total++] = pi;
    }
    if (total == 0) return (seq_t)1;

    uint64_t e1[A331731_MAXK + 2];
    uint64_t e2[A331731_MAXK + 2];
    for (size_t j = 0; j < A331731_MAXK + 2; ++j) { e1[j] = 0; e2[j] = 0; }

    /* A122111: conjugate partition. Prime p_r gets exponent v - prev, where
       r is the number of parts >= v and v runs over the distinct parts. */
    {
        size_t pos = 0;
        size_t remaining = total;
        uint64_t prev = 0;
        while (pos < total) {
            uint64_t v = ind[pos];
            size_t freq = 0;
            while (pos < total && ind[pos] == v) { ++freq; ++pos; }
            e1[remaining] += v - prev;
            prev = v;
            remaining -= freq;
        }
    }

    /* A241909 */
    if (total == 1) {
        e2[1] = ind[0];
    } else {
        e2[1] = ind[0] - 1;
        for (size_t j = 1; j + 1 < total; ++j)
            e2[j + 1] = ind[j] - ind[j - 1];
        e2[total] = 1 + ind[total - 1] - ind[total - 2];
    }

    /* gcd, dropping prime 2 (odd part) */
    seq_t result = (seq_t)1;
    for (size_t j = 2; j <= total; ++j) {
        uint64_t m = e1[j] < e2[j] ? e1[j] : e2[j];
        if (m > 0) {
            seq_t p = (seq_t)nth_prime((idx_t)j);
            result *= ipow_idx(p, (idx_t)m);
        }
    }
    return result;
}
