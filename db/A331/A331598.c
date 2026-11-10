/*
 * A331598  a(n) = A122111(n) / gcd(A122111(n),A241909(n)).
 * https://oeis.org/A331598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A122111(n) / gcd(A122111(n), A241909(n)). Both A122111(n)
 *            and A241909(n) are built as exponent vectors over the primes (no
 *            big intermediate products). For each prime p_k the result's
 *            exponent is max(0, e_A122111(k) - e_A241909(k))
 * Range:     u8 n <= 45, u16 n <= 117, u32 n <= 273, u64 n <= 625,
 *            u128 n <= 1453, big any n
 * Validated: standard, pass, 121 terms through n = 9625 (2026-11-08)
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

#define A331598_MAXK 160

/* k-th prime (1-based), k is small (<= number of prime factors + 1) */
static uint64_t A331598_kth_prime(size_t k)
{
    static uint64_t pr[A331598_MAXK + 1];
    static size_t sz = 0;
    uint64_t cand = (sz == 0) ? 2 : pr[sz - 1] + 1;
    while (sz < k) {
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            if (pr[i] * pr[i] > cand) break;
            if (cand % pr[i] == 0) { ok = false; break; }
        }
        if (ok) pr[sz++] = cand;
        ++cand;
    }
    return pr[k - 1];
}

static int A331598_cmp_idx(const void *x, const void *y)
{
    idx_t va = *(const idx_t *)x;
    idx_t vb = *(const idx_t *)y;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    idx_t indices[128];
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t pi = (idx_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) indices[total++] = pi;
    }
    if (total == 0) return (seq_t)1;
    qsort(indices, total, sizeof(idx_t), A331598_cmp_idx);

    /* exponent vectors indexed by prime index k (1..total) */
    uint64_t e1[A331598_MAXK];
    uint64_t e2[A331598_MAXK];
    memset(e1, 0, sizeof(e1));
    memset(e2, 0, sizeof(e2));

    /* A122111: conjugate partition */
    {
        idx_t remaining = (idx_t)total;
        idx_t last_part = 1;
        size_t pos = 0;
        while (pos < total) {
            idx_t current = indices[pos];
            size_t freq = 0;
            while (pos < total && indices[pos] == current) { ++freq; ++pos; }
            idx_t length = current - last_part + 1;
            e1[(size_t)remaining] += (uint64_t)length;
            remaining -= (idx_t)freq;
            last_part = current + 1;
        }
    }

    /* A241909 */
    if (total == 1) {
        e2[1] = (uint64_t)indices[0];          /* a(p_i) = 2^i */
    } else {
        for (size_t j = 0; j < total; ++j) {
            uint64_t e;
            if (j == 0) e = (uint64_t)(indices[0] - 1);
            else if (j == total - 1) e = (uint64_t)(1 + indices[j] - indices[j - 1]);
            else e = (uint64_t)(indices[j] - indices[j - 1]);
            e2[j + 1] += e;
        }
    }

    seq_t result = (seq_t)1;
    for (size_t k = 1; k <= total; ++k) {
        if (e1[k] > e2[k]) {
            uint64_t p = A331598_kth_prime(k);
            result *= ipow_idx((seq_t)p, (idx_t)(e1[k] - e2[k]));
        }
    }
    return result;
}
