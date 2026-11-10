/*
 * A331597  a(n) = A007947(A331595(n)).
 * https://oeis.org/A331597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(gcd(A122111(n), A241909(n))). Both A122111(n) and
 *            A241909(n) are built as prime-exponent vectors (indexed by prime
 *            index) so they never overflow; the gcd is the elementwise min,
 *            and its radical is the product of the primes p_k whose exponent
 *            is positive in both vectors
 * Range:     u8 n <= 1349, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9781 (2026-11-08)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

#define A331597_MAXK 130

static uint64_t A331597_kth_prime(size_t k)
{
    static uint64_t pr[A331597_MAXK + 1];
    static size_t cnt = 0;
    if (cnt == 0) {
        uint64_t c = 2;
        while (cnt < A331597_MAXK) {
            bool ok = true;
            for (size_t i = 0; i < cnt; ++i) {
                if (pr[i] * pr[i] > c) break;
                if (c % pr[i] == 0) { ok = false; break; }
            }
            if (ok) pr[cnt++] = c;
            ++c;
        }
    }
    return pr[k - 1];
}

static int A331597_cmp(const void *x, const void *y)
{
    idx_t va = *(const idx_t *)x;
    idx_t vb = *(const idx_t *)y;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t pcnt;
    factor_u64((uint64_t)n, pr, ex, &pcnt);

    idx_t ind[A331597_MAXK];
    size_t total = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t pi = (idx_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) ind[total++] = pi;
    }
    qsort(ind, total, sizeof(idx_t), A331597_cmp);

    uint64_t e1[A331597_MAXK + 1], e2[A331597_MAXK + 1];
    for (size_t k = 0; k <= A331597_MAXK; ++k) { e1[k] = 0; e2[k] = 0; }

    /* A122111 exponent vector */
    {
        size_t remaining = total;
        idx_t last_part = 1;
        size_t pos = 0;
        while (pos < total) {
            idx_t cur = ind[pos];
            size_t freq = 0;
            while (pos < total && ind[pos] == cur) { ++freq; ++pos; }
            e1[remaining] += (uint64_t)(cur - last_part + 1);
            remaining -= freq;
            last_part = cur + 1;
        }
    }

    /* A241909 exponent vector */
    if (total == 1) {
        e2[1] = (uint64_t)ind[0];
    } else {
        for (size_t j = 0; j < total; ++j) {
            uint64_t ee;
            if (j == 0) ee = (uint64_t)(ind[0] - 1);
            else if (j == total - 1) ee = (uint64_t)(1 + ind[j] - ind[j - 1]);
            else ee = (uint64_t)(ind[j] - ind[j - 1]);
            e2[j + 1] += ee;
        }
    }

    seq_t result = (seq_t)1;
    for (size_t k = 1; k <= total; ++k) {
        if (e1[k] > 0 && e2[k] > 0)
            result *= (seq_t)A331597_kth_prime(k);
    }
    return result;
}
