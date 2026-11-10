/*
 * A329033  a(n) = A003415(A122111(n)).
 * https://oeis.org/A329033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A122111(n)), with A122111(n) = prod q_i^e_i built
 *            from the conjugate partition of the prime indices of n; a(n) =
 *            sum_i e_i * q_i^(e_i-1) * prod_{j!=i} q_j^e_j (a(1) = 0)
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 108, u64 n <= 280,
 *            u128 n <= 676, big any n
 * Validated: standard, pass, 86 terms through n = 3773 (2026-11-08)
 * Helpers:   factor_u64, ipow_idx, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"

static uint64_t A329033_kth_prime(idx_t k)
{
    uint64_t cnt = 0;
    uint64_t p = 1;
    while (cnt < (uint64_t)k) {
        p++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= p; d++) {
            if (p % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    idx_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t ix = (idx_t)prime_pi(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e)
            indices[idx_cnt++] = ix;
    }

    /* insertion sort ascending */
    for (size_t i = 1; i < idx_cnt; ++i) {
        idx_t v = indices[i];
        size_t j = i;
        while (j > 0 && indices[j - 1] > v) {
            indices[j] = indices[j - 1];
            --j;
        }
        indices[j] = v;
    }

    /* build factorization of A122111(n): distinct primes q[i] with exponents ex[i] */
    uint64_t q[128];
    idx_t ex[128];
    size_t gc = 0;
    idx_t remaining = (idx_t)idx_cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        idx_t current = indices[pos];
        idx_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        q[gc] = A329033_kth_prime(remaining);
        ex[gc] = current - last_part + 1;
        ++gc;
        remaining -= freq;
        last_part = current + 1;
    }

    seq_t total = 0;
    for (size_t i = 0; i < gc; ++i) {
        seq_t term = (seq_t)ex[i];
        term *= ipow_idx((seq_t)q[i], ex[i] - 1);
        for (size_t j = 0; j < gc; ++j) {
            if (j == i) continue;
            term *= ipow_idx((seq_t)q[j], ex[j]);
        }
        total += term;
    }
    return total;
}
