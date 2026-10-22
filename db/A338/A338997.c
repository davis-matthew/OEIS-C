/*
 * A338997  Number of (i,j,k) in {1,2,...,n}^3 such that gcd(n,i) = gcd(n,j) =
 *          gcd(n,k).
 * https://oeis.org/A338997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{d|n} φ(d)^3
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

/* ------------------------------------------------------------------ */
/* Recursive helper that runs through all divisors of n                */
static void _divisor_sum(const uint64_t *pr,
                         const uint64_t *ex,
                         size_t pc,
                         size_t idx,
                         uint64_t cur,
                         seq_t *sum)
{
    if (idx == pc) {
        /* cur is a divisor of n */
        seq_t phi_cur = phi((idx_t)cur);          /* φ(cur) */
        seq_t phi3 = phi_cur * phi_cur * phi_cur;
        *sum += phi3;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t cur_pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        _divisor_sum(pr, ex, pc, idx + 1, cur * cur_pow, sum);
        cur_pow *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0)                  /* guard for an illegal call */
        return 0;

    /* 64 is plenty for the number of distinct prime factors of n ≤ 10⁴. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pc = 0;

    factor_u64((uint64_t)n, primes, exps, &pc);

    seq_t sum = 0;
    _divisor_sum(primes, exps, pc, 0, 1, &sum);
    return sum;
}
