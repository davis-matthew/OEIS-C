/*
 * A242404  a(n) = Sum over i and j with j<=i and i=1.. sopf(n) of
 *          binomial(d(i), d(j)), where d(i) are the prime divisors of n and
 *          sopf(n) = sum of the distinct primes dividing n (A008472).
 * https://oeis.org/A242404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=j<=i<=k} binomial(p_i, p_j), where p_i are the
 *            distinct prime divisors of n. The sequence starts at a(1)=0
 * Range:     u8 n <= 38, u16 n <= 144, u32 n <= 472, u64 n <= 1816,
 *            u128 n <= 6682, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* gcd for seq_t (works for both native integers and arbitrary-precision
   integer classes). */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Multiplicative binomial coefficient: binom(n, k).  The algorithm
   cancels common factors at each step to avoid overflow. */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd_seq_t(num, den);
        num /= g;
        den /= g;

        g = gcd_seq_t(res, den);
        res /= g;
        den /= g;  /* den should now be 1 */

        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    /* collect distinct prime divisors of n */
    uint64_t primes[64];
    size_t   pcnt = 0;
    uint64_t m = n;

    /* factor out 2 */
    if ((m & 1) == 0) {
        primes[pcnt++] = 2;
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            primes[pcnt++] = p;
            while (m % p == 0)
                m /= p;
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1)
        primes[pcnt++] = m;

    /* sum over all unordered pairs of the distinct primes */
    seq_t sum = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            sum += my_binom((idx_t)primes[i], (idx_t)primes[j]);
        }
    }
    return sum;
}
