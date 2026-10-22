/*
 * A389465  Number of natural numbers in range [n .. sigma(n)] that are
 *          coprime to n.
 * https://oeis.org/A389465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=n..sigma(n)} [1 == gcd(k,n)]
 * Range:     u8 n <= 431, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* recursively enumerate all square‑free divisors of n and
   accumulate \sum mu(d) * floor(m / d)  */
static void dfs(size_t pos, idx_t cur, int parity,
                const uint64_t *pr, size_t cnt,
                seq_t m, seq_t *sum)
{
    if (pos == cnt) {
        seq_t val = m / (seq_t)cur;
        if (parity == 1)
            *sum += val;
        else
            *sum -= val;
        return;
    }
    /* skip this prime */
    dfs(pos + 1, cur, parity, pr, cnt, m, sum);
    /* include this prime (square‑free) */
    dfs(pos + 1, cur * pr[pos], -parity, pr, cnt, m, sum);
}

/* Count of integers k <= m that are coprime to n.  */
static seq_t count_coprime(idx_t n, seq_t m)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(0, (idx_t)1, 1, primes, cnt, m, &sum);
    return sum;
}

/* OEIS sequence A389465: number of k in [n .. sigma(n)] coprime to n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for out of bounds */
    seq_t sigma = sigma_of(n);
    seq_t total = count_coprime(n, sigma);
    if (n == 1) return total;
    seq_t below = count_coprime(n, (seq_t)(n - 1));
    return total - below;
}
