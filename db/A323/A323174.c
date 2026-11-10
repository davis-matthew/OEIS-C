/*
 * A323174  Deficiency computed for conjugated prime factorization: a(n) =
 *          A033879(A122111(n)).
 * https://oeis.org/A323174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A033879(A122111(n)) = 2*m - sigma(m), m = A122111(n) =
 *            product over j>=1 of prime(c_j), c_j = number of prime-factor
 *            indices of n that are >= j
 * Range:     i8 n <= 37, i16 n <= 105, i32 n <= 261, i64 n <= 621,
 *            i128 n <= 1437, big any n
 * Validated: standard, pass, 96 terms through n = 9788 (2026-11-08)
 * Helpers:   factor_u64, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pc = 0;
    factor_u64((uint64_t)n, pr, ex, &pc);

    uint64_t maxidx = 0;
    uint64_t idxs[64];
    for (size_t i = 0; i < pc; ++i) {
        idxs[i] = (uint64_t)prime_pi((idx_t)pr[i]);
        if (idxs[i] > maxidx) maxidx = idxs[i];
    }
    if (maxidx == 0) return (seq_t)1;

    uint64_t *cnt = (uint64_t *)calloc((size_t)maxidx + 2, sizeof(uint64_t));
    if (!cnt) abort();
    for (size_t i = 0; i < pc; ++i)
        cnt[idxs[i]] += ex[i];
    /* suffix sums: cnt[j] = number of indices >= j */
    for (uint64_t j = maxidx; j >= 1; --j) {
        cnt[j] += cnt[j + 1];
    }

    seq_t m = 1, sig = 1;
    uint64_t j = 1;
    while (j <= maxidx) {
        uint64_t c = cnt[j];
        uint64_t e = 0;
        while (j <= maxidx && cnt[j] == c) { ++e; ++j; }
        seq_t p = (seq_t)nth_prime((idx_t)c);
        seq_t term = 1, s = 1;
        for (uint64_t k = 0; k < e; ++k) {
            term *= p;
            s += term;
        }
        m *= term;
        sig *= s;
    }
    free(cnt);
    return (seq_t)2 * m - sig;
}
