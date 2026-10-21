/*
 * A243016  Number of solutions for k*n/(k+n) = p for integer k > 0 and prime
 *          p.
 * https://oeis.org/A243016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p<n such that (n-p) divides n^2
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of primes p<n with (n-p) | n^2.
   Enumerate all divisors d of n^2, d<n, let p=n-d and test p for primality. */
#include <stdint.h>
#include <stdbool.h>

/* Small prime test (trial division).  Works for the small values needed. */
static bool is_prime_small(idx_t v)
{
    if (v < 2) return false;
    for (idx_t d = 2; d * d <= v; ++d)
        if (v % d == 0) return false;
    return true;
}

/* Recursive enumeration of all divisors of n^2.  pf[] contains the prime factors
   of n and exp[] their exponents.
   Property: each divisor d of n^2 can be written as product pf[i]^e_i with
   0 ≤ e_i ≤ 2*exp[i]. */
static void dfs_divisors(int idx, seq_t cur,
                         const idx_t *pf, const int *exp, int total,
                         const seq_t n_val, seq_t *cnt)
{
    if (idx == total) {
        if (cur < n_val) {
            seq_t diff = n_val - cur;              /* diff = p */
            idx_t p = (idx_t)diff;
            if (is_prime_small(p))
                ++(*cnt);
        }
        return;
    }
    seq_t val = 1;
    int maxe = 2 * exp[idx];
    for (int e = 0; e <= maxe; ++e) {
        dfs_divisors(idx + 1, cur * val, pf, exp, total, n_val, cnt);
        val *= (seq_t)pf[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1) return (seq_t)0;

    /* Factor n into prime powers */
    const int MAX_FACTORS = 32;                /* plenty for any n up to 64‑bit */
    idx_t pf[MAX_FACTORS];
    int exp[MAX_FACTORS];
    int num_factors = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[num_factors] = p;
            exp[num_factors] = e;
            ++num_factors;
        }
    }
    if (temp > 1) {    /* remaining prime factor */
        pf[num_factors] = temp;
        exp[num_factors] = 1;
        ++num_factors;
    }

    /* Enumerate divisors of n^2 and count suitable primes */
    seq_t count = 0;
    seq_t n_val = (seq_t)n;
    dfs_divisors(0, (seq_t)1, pf, exp, num_factors, n_val, &count);
    return count;
}
