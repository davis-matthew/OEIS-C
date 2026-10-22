/*
 * A351612  Number of ordered pairs, (s,t), 1 <= s <= t, such that (t^s) | n.
 * https://oeis.org/A351612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of ordered pairs (s,t), 1 ≤ s ≤ t, such that t^s |
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factorises n, enumerates all its divisors, and for each
   divisor t counts the largest exponent s with t^s | n.  The contribution
   for t is min(s, t). */

#include <stdint.h>
#include <stddef.h>

static void dfs(size_t idx, uint64_t cur, const uint64_t *pr,
                const uint64_t *ex, size_t cnt, uint64_t n, seq_t *acc)
{
    if (idx == cnt) {
        if (cur == 1) {
            *acc += (seq_t)1;          /* t = 1 contributes exactly one pair (1,1) */
            return;
        }
        uint64_t tmp = n;
        unsigned s = 0;
        while (tmp % cur == 0) {
            tmp /= cur;
            ++s;
        }
        /* s ≥ 1 because cur divides n */
        if ((seq_t)s < cur) {
            *acc += (seq_t)s;
        } else {
            *acc += (seq_t)cur;
        }
        return;
    }

    uint64_t p   = pr[idx];
    uint64_t pow = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        dfs(idx + 1, cur * pow, pr, ex, cnt, n, acc);
        pow *= p;
    }
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 (outside OEIS offset); returns 0. */
    if (n == 0) return 0;

    uint64_t m = (uint64_t)n;
    /* Prime factorisation of m (n) – works for all 64‑bit values. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt = 0;
    uint64_t tmp = m;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                /* Remaining prime factor > 1 */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 0;
    dfs(0, 1, primes, exps, cnt, m, &result);
    return result;
}
