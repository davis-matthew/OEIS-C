/*
 * A327172  If there is a divisor d of n such that phi(d)*d = n, then a(n) =
 *          d, otherwise a(n) = 0.
 * https://oeis.org/A327172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9919, u16 n <= 9919, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A327172: a(n) = d where d | n and φ(d)*d == n, else 0 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

static void search_rec(const uint64_t *pr, const uint64_t *ex, size_t idx, size_t total,
                       idx_t cur, idx_t n_val, seq_t *found)
{
    if (*found) return;
    if (idx == total) {
        seq_t phi_cur = phi(cur);
        seq_t prod = phi_cur * ((seq_t)cur);
        if (prod == (seq_t)n_val) {
            *found = cur;
        }
        return;
    }
    idx_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        search_rec(pr, ex, idx + 1, total, cur * mult, n_val, found);
        mult *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);
    seq_t result = 0;
    search_rec(primes, exps, 0, cnt, (idx_t)1, n, &result);
    return result;
}
