/*
 * A318874  Number of divisors d of n for which 2*phi(d) > d.
 * https://oeis.org/A318874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [2*phi(d) > d]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive enumeration of all divisors of n. */
static void dfs(size_t idx, size_t total, const uint64_t *pr, const uint64_t *ex,
                idx_t cur_d, idx_t cur_phi, idx_t *cnt)
{
    if (idx == total) {
        if (cur_phi > cur_d / 2)
            ++(*cnt);
        return;
    }

    idx_t pow = 1;
    /* The exponent stored in ex[idx] is a uint64_t.  It is small, so the
       cast to size_t in the loop bound is safe. */
    for (size_t e = 0; e <= ex[idx]; ++e) {
        idx_t next_d = cur_d * pow;

        idx_t phi_factor;
        if (e == 0) {
            phi_factor = 1;
        } else {
            /* pow = p^e; previous power is pow/p. */
            phi_factor = (pow / pr[idx]) * (pr[idx] - 1);
        }

        idx_t next_phi = cur_phi * phi_factor;
        dfs(idx + 1, total, pr, ex, next_d, next_phi, cnt);
        pow *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt_factors = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt_factors);

    idx_t count = 0;
    dfs(0, cnt_factors, pr, ex, 1, 1, &count);
    return (seq_t)count;
}
