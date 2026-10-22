/*
 * A336500  Number of divisors d|n with distinct prime multiplicities such
 *          that the quotient n/d also has distinct prime multiplicities.
 * https://oeis.org/A336500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d|n such that d and n/d both have
 *            distinct prime multiplicities
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* recursive helper to enumerate all divisors of n */
static void dfs(size_t pos, uint64_t val, uint64_t *m,
                const uint64_t *pr, const uint64_t *exp, size_t nf,
                seq_t *cnt)
{
    if (pos == nf) {
        /* check distinctness for divisor part */
        bool ok = true;
        for (size_t i = 0; i < nf && ok; ++i) {
            if (m[i] == 0) continue;
            for (size_t j = i + 1; j < nf; ++j) {
                if (m[j] == 0) continue;
                if (m[i] == m[j]) { ok = false; break; }
            }
        }
        /* check distinctness for co‑divisor part */
        for (size_t i = 0; i < nf && ok; ++i) {
            uint64_t r = exp[i] - m[i];
            if (r == 0) continue;
            for (size_t j = i + 1; j < nf; ++j) {
                uint64_t r2 = exp[j] - m[j];
                if (r2 == 0) continue;
                if (r == r2) { ok = false; break; }
            }
        }
        if (ok) (*cnt) += 1;
        return;
    }

    uint64_t p = pr[pos];
    uint64_t e = exp[pos];
    uint64_t mul = 1;
    for (uint64_t k = 0; k <= e; ++k) {
        m[pos] = k;
        dfs(pos + 1, val * mul, m, pr, exp, nf, cnt);
        mul *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but harmless */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t  cnt = 0;
    uint64_t t = nn;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (t > 1) {
        primes[cnt] = t;
        exps[cnt]   = 1;
        ++cnt;
    }

    uint64_t m[32];
    seq_t result = (seq_t)0;
    dfs((size_t)0, 1ULL, m, primes, exps, cnt, &result);
    return result;
}
