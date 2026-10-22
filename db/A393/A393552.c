/*
 * A393552  Number of distinct line sets in an n-gon connecting vertices and
 *          midpoints, disallowing same-edge connections, up to rotations and
 *          reflections.
 * https://oeis.org/A393552
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * ( 2^floor((n*(2*n-3)-3)/2) +
 *            2^floor((n*(2*n-3)-2)/2) + (1/n) * Σ_{d|n} φ(d) * 2^((2*n-3 - (d
 *            % 2)) * n / d) )
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   factor_u64, phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

/* Recursive enumeration of all divisors of n and accumulation of
   φ(d) * 2^((2*n-3 - d%2) * n / d) into *sum.  */
static void rec_divs(const uint64_t *primes, const uint64_t *exps,
                     size_t pcnt, size_t idx, uint64_t cur,
                     idx_t n, seq_t *sum)
{
    if (idx == pcnt) {
        seq_t phi_cur = phi((idx_t)cur);
        idx_t mult = 2 * n - 3 - (cur & 1ULL);
        idx_t term = mult * (n / cur);
        seq_t pw = pow2(term);
        *sum += phi_cur * pw;
        return;
    }

    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        rec_divs(primes, exps, pcnt, idx + 1, cur, n, sum);
        cur *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Handle indices below the offset.  */
    if (n < 3) return 0;

    /* 2^floor((n*(2*n-3)-3)/2) + 2^floor((n*(2*n-3)-2)/2)  */
    idx_t t = n * (2 * n - 3);
    idx_t floor1 = (t - 3) / 2;
    idx_t floor2 = (t - 2) / 2;

    seq_t part1 = pow2(floor1);
    seq_t part2 = pow2(floor2);

    /* Sum over divisors of n.  */
    seq_t sumDiv = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    rec_divs(primes, exps, pcnt, 0, 1ULL, n, &sumDiv);

    seq_t sumDiv_n = sumDiv / (seq_t)n;
    seq_t total = part1 + part2 + sumDiv_n;
    return total / 2;
}
