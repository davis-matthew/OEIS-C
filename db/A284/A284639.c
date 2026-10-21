/*
 * A284639  Number of ways to write n > 1 as a power of a product n =
 *          (x_1*..*x_k)^y, where y >= 1, x_i > 1, and x_i <= x_j for i < j.
 * https://oeis.org/A284639
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A284639: Number of ways to write n > 1 as (x1*…*xk)^y, y≥1, xi>1,
   xi ≤ xj for i<j.  The formula implemented is:
        a(n) = Σ_{d | G}  f( n^{1/d} )
   where G = gcd of exponents in the prime factorisation of n,
   and f(m) is the number of unordered factorizations of m into
   integers > 1 (i.e. multiplicative partitions of m) with factors in
   non‑decreasing order.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Small list of primes ≤ 100 (sufficient for n ≤ 10000). */
static const uint32_t primes_list[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t primes_cnt = sizeof(primes_list) / sizeof(primes_list[0]);

/* gcd for idx_t (unsigned). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Recursively count unordered factorizations of m (m ≥ 2) into factors >1,
   with current minimum allowed factor min_factor.  The representation
   consisting of the single factor m itself is counted once. */
static seq_t count_partitions(uint64_t m, uint64_t min_factor)
{
    seq_t total = 0;
    for (uint64_t d = min_factor; d * d <= m; ++d) {
        if (m % d == 0) {
            total += count_partitions(m / d, d);
        }
    }
    /* Representation consisting of m itself. */
    total += (seq_t)1;
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined for n == 1, but harmless */
    /* Factor n into primes and exponents. */
    uint64_t primes[10];
    idx_t    exps[10];
    size_t   pcnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_list[i] * primes_list[i] <= tmp; ++i) {
        uint32_t p = primes_list[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute G = gcd of all exponents. */
    idx_t G = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        G = gcd_idx(G, exps[i]);
    }
    if (G == 0) G = 1;    /* should not happen for n > 1 */

    /* Sum over all divisors d of G. */
    seq_t total = 0;
    for (idx_t d = 1; d <= G; ++d) {
        if (G % d != 0) continue;
        /* Compute M = product of primes[i]^(exps[i]/d). */
        uint64_t M = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            idx_t exp_div = exps[i] / d;
            for (idx_t k = 0; k < exp_div; ++k) {
                M *= primes[i];
            }
        }
        total += count_partitions(M, 2);   /* all factorizations of M */
    }
    return total;
}
