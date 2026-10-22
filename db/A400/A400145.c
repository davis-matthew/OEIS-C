/*
 * A400145  The sum of divisors d of n such that gcid(d, n/d) is odd, where
 *          gcid is the greatest common infinitary divisor.
 * https://oeis.org/A400145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum_{d|n, gcid(d, n/d) odd} d
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where gcid(d, n/d) is the greatest common infinitary divisor,
    i.e. for prime 2 the exponent is bitwise AND of the two
    exponents.  The condition is equivalent to (e(d,2) &
    (e(n,2)-e(d,2))) == 0.  */
/*   The implementation factors n by trial division, enumerates all
    divisors by recursion, and sums only those whose exponent of
    2 satisfies the above condition. */

#include <stdint.h>
#include <stddef.h>

/* local factorial helper: factorise n into primes and exponents */
static void factor_u64(uint64_t n, uint64_t *primes, uint32_t *exps, size_t *cnt)
{
    size_t c = 0;
    if (n == 0) {
        *cnt = 0;
        return;
    }
    uint32_t e = 0;
    while ((n & 1ULL) == 0) {    /* factor out 2 */
        n >>= 1;
        ++e;
    }
    if (e > 0) {
        primes[c] = 2;
        exps[c] = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of divisors */
static void dfs(size_t idx, seq_t curr, unsigned int e2_now,
                const uint64_t *primes, const uint32_t *exps,
                size_t cnt, unsigned int e2_total, seq_t *sum)
{
    if (idx == cnt) {
        unsigned int diff = e2_total - e2_now;
        if ((e2_now & diff) == 0) {
            *sum += curr;
        }
        return;
    }
    uint64_t p = primes[idx];
    unsigned int emax = exps[idx];
    seq_t val = curr;
    unsigned int e2 = e2_now;
    dfs(idx + 1, val, e2, primes, exps, cnt, e2_total, sum); /* exponent 0 */

    for (unsigned int k = 1; k <= emax; ++k) {
        val *= (seq_t)p;                     /* multiply by p once more */
        if (p == 2) {
            ++e2;                           /* increase exponent of 2 */
        }
        dfs(idx + 1, val, e2, primes, exps, cnt, e2_total, sum);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint32_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned int e2_total = 0;
    if (cnt && primes[0] == 2) {
        e2_total = exps[0];
    }

    seq_t sum = (seq_t)0;
    dfs(0, (seq_t)1, 0, primes, exps, cnt, e2_total, &sum);
    return sum;
}
