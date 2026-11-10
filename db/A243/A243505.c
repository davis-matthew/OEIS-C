/*
 * A243505  Permutation of natural numbers, take the odd bisection of A122111
 *          and divide the largest prime factor out: a(n) =
 *          A052126(A122111(2n-1)).
 * https://oeis.org/A243505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A122111(A064216(n)), where A064216(n) replaces each prime
 *            p of 2n-1 by prevprime(p), and A122111 maps prime indices to the
 *            conjugate partition
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 68, u64 n <= 156,
 *            u128 n <= 363, big any n
 * Validated: standard, pass, 69 terms through n = 997 (2026-11-07)
 * Helpers:   factor_u64, ipow_idx, is_prime_u64, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"
#include "../utils/is_prime_u64.h"

static uint64_t A243505_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !is_prime_u64(q)) --q;
    return q;
}

static uint64_t A243505_A064216(uint64_t n)
{
    uint64_t t = 2ULL * n - 1ULL;
    if (t <= 1) return 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(t, pr, ex, &cnt);
    uint64_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = A243505_prevprime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) res *= q;
    }
    return res;
}

static seq_t A243505_A122111(uint64_t m)
{
    if (m == 1) return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t pcnt = 0;
    factor_u64(m, pr, ex, &pcnt);

    idx_t indices[128];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) indices[cnt++] = idx;
    }
    if (cnt == 0) return (seq_t)1;

    /* insertion sort ascending */
    for (size_t i = 1; i < cnt; ++i) {
        idx_t v = indices[i];
        size_t j = i;
        while (j > 0 && indices[j - 1] > v) { indices[j] = indices[j - 1]; --j; }
        indices[j] = v;
    }

    seq_t result = (seq_t)1;
    idx_t remaining = (idx_t)cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < cnt) {
        idx_t current = indices[pos];
        size_t freq = 0;
        while (pos < cnt && indices[pos] == current) { ++freq; ++pos; }
        idx_t length = current - last_part + 1;
        seq_t pv = (seq_t)nth_prime(remaining);
        result = result * ipow_idx(pv, length);
        remaining -= (idx_t)freq;
        last_part = current + 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A243505_A122111(A243505_A064216((uint64_t)n));
}
