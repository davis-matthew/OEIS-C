/*
 * A322865  a(n) = A000265(A122111(n)).
 * https://oeis.org/A322865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265(A122111(n)). A122111(n) is the product of primes
 *            p_r^(i_j - i_{j-1}), where i_1 <= ... <= i_k are the prime
 *            indices of n and r = number of parts >= i_j. The factor of 2 (r
 *            = 1) is never multiplied in, so values that fit in seq_t stay
 *            exact
 * Range:     u8 n <= 168, u16 n <= 960, u32 n <= 5328, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9423 (2026-11-08)
 * Helpers:   factor_u64, ipow_idx, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi_u64.h"

static uint32_t A322865_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    uint32_t cand;

    if (pr == NULL) {
        cap = 64;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
        pr[0] = 2;
        sz = 1;
    }
    cand = (sz == 1) ? 3 : pr[sz - 1] + 2;
    while (sz < (size_t)k) {
        bool ok = true;
        size_t i;
        for (i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)cand) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) {
            if (sz == cap) {
                cap <<= 1;
                pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
                if (!pr) abort();
            }
            pr[sz++] = cand;
        }
        cand += 2;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    idx_t indices[128];
    size_t idx_cnt = 0;
    size_t i, j, pos;
    seq_t result = (seq_t)1;
    idx_t remaining, last_part;

    if (n <= 1) return (seq_t)1;

    factor_u64((uint64_t)n, primes, exps, &pcnt);
    for (i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi_u64(primes[i]);
        uint64_t e;
        for (e = 0; e < exps[i]; ++e)
            indices[idx_cnt++] = idx;
    }
    if (idx_cnt == 0) return (seq_t)1;

    /* insertion sort ascending */
    for (i = 1; i < idx_cnt; ++i) {
        idx_t v = indices[i];
        j = i;
        while (j > 0 && indices[j - 1] > v) {
            indices[j] = indices[j - 1];
            --j;
        }
        indices[j] = v;
    }

    remaining = (idx_t)idx_cnt;
    last_part = 1;
    pos = 0;
    while (pos < idx_cnt) {
        idx_t current = indices[pos];
        idx_t freq = 0;
        idx_t length;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        length = current - last_part + 1;
        if (remaining > 1 && length > 0) {
            seq_t pv = (seq_t)A322865_get_prime(remaining);
            result = result * ipow_idx(pv, length);
        }
        remaining -= freq;
        last_part = current + 1;
    }
    return result;
}
