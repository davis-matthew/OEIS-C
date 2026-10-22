/*
 * A302986  Number of partitions of n into two distinct parts (p,q) such that
 *          p, q and |q-p| are all squarefree.
 * https://oeis.org/A302986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} μ(i)²·μ(n-i)²·μ(n-2i)²
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(m) is the Möbius function (A008683).  */

/* The function below computes mu(m) for all m ≤ n by a linear sieve
   and then evaluates the sum.  A static cache is used so successive
   calls are fast.  The routine works for any idx_t/seq_t that the
   build environment supplies.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static cache for Möbius values: μ[0] unused, μ[1] = 1.  */
static int8_t *mu_cache = NULL;          /* signed char, values −1,0,+1 */
static idx_t      mu_limit = 0;           /* highest index cached */

/* Helper: free previously allocated cache memory */
static void free_mu_cache(void)
{
    if (mu_cache)
    {
        free(mu_cache);
        mu_cache = NULL;
        mu_limit = 0;
    }
}

/* Build μ[1..n] using a linear sieve.  */
static void build_mu_cache(idx_t n)
{
    free_mu_cache();                      /* start fresh */

    /* allocate arrays */
    mu_cache   = (int8_t *)malloc((n + 1) * sizeof(int8_t));
    if (!mu_cache) return;

    /* min_prime[i] = smallest prime divisor of i (0 means i unmarked) */
    idx_t *min_prime = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!min_prime) { free(mu_cache); mu_cache = NULL; return; }

    /* dynamic array of primes */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    size_t primes_cap = 0;

    mu_cache[0] = 0;
    mu_cache[1] = 1;
    memset(min_prime, 0, (n + 1) * sizeof(idx_t));

    for (idx_t i = 2; i <= n; ++i)
    {
        if (min_prime[i] == 0)          /* i is prime */
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) break;
            }
            primes[primes_cnt++] = i;
            mu_cache[i] = -1;           /* μ(p) = −1 for prime */
            min_prime[i] = i;
        }

        for (size_t k = 0; k < primes_cnt; ++k)
        {
            idx_t p = primes[k];
            if (p > min_prime[i]) break;
            idx_t ip = i * p;
            if (ip > n) break;
            min_prime[ip] = p;
            if (p == min_prime[i])
                mu_cache[ip] = 0;          /* square factor → μ = 0 */
            else
                mu_cache[ip] = -mu_cache[i];
        }
    }

    free(min_prime);
    free(primes);

    mu_limit = n;
}

/* Ensure cache contains μ[1..n] */
static void ensure_mu_cache(idx_t n)
{
    if (mu_limit < n) build_mu_cache(n);
}

/* ------------------------------------------------------------------ */
/* Main function requested by the build system                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* floor((n-1)/2)==0 */

    ensure_mu_cache(n);

    idx_t max_i = (n - 1) / 2;
    seq_t sum = 0;

    for (idx_t i = 1; i <= max_i; ++i)
    {
        /* μ(x)² = 1 iff μ(x) ≠ 0 */
        if (mu_cache[i] && mu_cache[n - i] && mu_cache[n - 2 * i])
            sum += 1;
    }

    return sum;
}
