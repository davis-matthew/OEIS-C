/*
 * A352053  Sum of the 7th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 * Sum_{d|n, d<n, d odd} 1/d^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = Sum_{odd d|n, d<n} (n/d)^7 */
#include <stdint.h>
#include <stddef.h>

/* Compute v^7 using integer arithmetic (modulo seq_t space) */
static inline seq_t pow7(seq_t v)
{
    seq_t r = v;
    r *= v;  /* v^2 */
    r *= v;  /* v^3 */
    r *= v;  /* v^4 */
    r *= v;  /* v^5 */
    r *= v;  /* v^6 */
    r *= v;  /* v^7 */
    return r;
}

/* Recursive enumeration of all odd divisors of the odd part of n */
static void dfs_divisors(size_t idx, size_t prime_cnt,
                         uint64_t primes[], int exps[],
                         uint64_t cur, idx_t n, seq_t *sum)
{
    if (idx == prime_cnt) {
        if (cur < (uint64_t)n) {
            seq_t t = (seq_t)(n / cur);
            *sum += pow7(t);
        }
        return;
    }
    uint64_t p = primes[idx];
    int emax = exps[idx];
    uint64_t prod = 1;
    for (int e = 0; e <= emax; ++e) {
        dfs_divisors(idx + 1, prime_cnt, primes, exps,
                     cur * prod, n, sum);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Compute the odd part of n */
    uint64_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* Factor the odd part */
    uint64_t primes[16];
    int exps[16];
    size_t prime_cnt = 0;
    uint64_t temp = m;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[prime_cnt] = p;
            exps[prime_cnt] = e;
            ++prime_cnt;
        }
    }
    if (temp > 1 && prime_cnt < 16) {
        primes[prime_cnt] = temp;
        exps[prime_cnt] = 1;
        ++prime_cnt;
    }

    seq_t sum = 0;
    dfs_divisors(0, prime_cnt, primes, exps, 1, n, &sum);
    return sum;
}
