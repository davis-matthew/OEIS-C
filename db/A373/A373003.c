/*
 * A373003  a(n) = (1/n) * Sum_{k=1..n} tau( (n/gcd(k,n))^n ).
 * https://oeis.org/A373003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(d) * tau(d^n)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAXLIMIT 100000

/* Prime table (up to MAXLIMIT) */
static unsigned char  is_prime[MAXLIMIT + 1];
static unsigned int   prime_table[12000];
static size_t         prime_count = 0;
static int            primes_initialized = 0;

/* Sieve to build prime_table once */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    primes_initialized = 1;
    for (size_t i = 0; i <= (size_t)MAXLIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= (size_t)MAXLIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)MAXLIMIT; j += i)
                is_prime[j] = 0;
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)MAXLIMIT; ++i)
        if (is_prime[i])
            prime_table[idx++] = (unsigned int)i;
    prime_count = idx;
}

/* Recursive enumeration of divisors of n, accumulating  Σ φ(d)·τ(dⁿ) */
static void dfs_div(int idx, const idx_t *primes, const int *exps,
                    size_t pcnt, idx_t n,
                    seq_t phi, seq_t tau, seq_t *sum)
{
    if ((size_t)idx == pcnt) {
        *sum += phi * tau;
        return;
    }
    idx_t p = primes[idx];
    int    e = exps[idx];
    seq_t powp = 1;               /* p^k, k starts at 0 */

    /* k = 0 */
    dfs_div(idx + 1, primes, exps, pcnt, n, phi, tau, sum);

    for (int k = 1; k <= e; ++k) {
        powp *= p;                                     /* now powp = p^k */
        seq_t phi_factor = (seq_t)(p - 1) * (powp / p); /* (p-1)·p^(k-1) */
        seq_t phi_new = phi * phi_factor;
        seq_t tau_new = tau * (seq_t)(k * n + 1);      /* (k·n + 1) */
        dfs_div(idx + 1, primes, exps, pcnt, n,
                phi_new, tau_new, sum);
    }
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, but return 0 for safety */

    init_primes();

    /* Factor n */
    idx_t tmp = n;
    size_t factor_cnt = 0;
    idx_t  prime_factors[20];
    int    exponents[20];
    for (size_t i = 0; i < prime_count && (idx_t)prime_table[i] * prime_table[i] <= tmp; ++i) {
        unsigned int p = prime_table[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            prime_factors[factor_cnt] = (idx_t)p;
            exponents[factor_cnt]     = exp;
            ++factor_cnt;
        }
    }
    if (tmp > 1) {
        prime_factors[factor_cnt] = tmp;
        exponents[factor_cnt]     = 1;
        ++factor_cnt;
    }

    seq_t sum = 0;
    dfs_div(0, prime_factors, exponents, factor_cnt, n, 1, 1, &sum);
    return sum / (seq_t)n;
}
