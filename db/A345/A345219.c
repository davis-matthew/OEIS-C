/*
 * A345219  Number of divisors d of n with an odd number of primes not
 *          exceeding d.
 * https://oeis.org/A345219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (pi(d) mod 2)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Number of divisors d of n such that the number of primes ≤ d is odd. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_pi.h"
#include "../utils/factor_u64.h"

/* Recursively count divisors with odd pi(d). */
static seq_t count_odd_pi_divisors(uint64_t cur, int idx,
                                   uint64_t *primes, uint64_t *exps, int cnt)
{
    if (idx == cnt) {
        seq_t p = compute_pi(cur);
        return p & 1;
    }
    seq_t sum = 0;

    /* case e = 0 (prime not used) */
    sum += count_odd_pi_divisors(cur, idx + 1, primes, exps, cnt);

    /* cases e = 1..exp */
    uint64_t val = cur;
    for (int e = 1; e <= (int)exps[idx]; ++e) {
        val *= primes[idx];
        sum += count_odd_pi_divisors(val, idx + 1, primes, exps, cnt);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, return 0 */
    uint64_t N = (uint64_t)n;

    /* factor N; we anticipate at most 64 distinct primes for 64‑bit numbers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(N, primes, exps, &cnt);

    return count_odd_pi_divisors(1, 0, primes, exps, (int)cnt);
}
