/*
 * A269301  Normalization coefficients for quantum Pascal's pyramid,
 *          numerators of: T(n,k,m) = ((n - m)! m!)/(2^n (n - k)! k!).
 * https://oeis.org/A269301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of * ((n - m)! m!)/(2^n (n - k)! k!) *
 *            after reducing the fraction, where (n,k,m) are obtained from
 *            index n via * the block indexing scheme: x(n,k,m) = Σ_{i=0}^n
 *            i^2 + k (n + 1) + m
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* Prime sieve (simple trial division) with small static table */
/* ------------------------------------------------------------ */
#define MAX_PRIMES 20000

static size_t prime_cnt = 0;        /* number of primes stored */
static idx_t   prime_last = 1;      /* last candidate checked (initially 1) */
static idx_t   primes[MAX_PRIMES];  /* prime cache */

/* ensure that each prime ≤ limit is present in primes[] */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_last)
        return;

    for (idx_t cand = prime_last + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= MAX_PRIMES)
                exit(EXIT_FAILURE);
            primes[prime_cnt++] = cand;
        }
    }
    prime_last = limit;
}

/* ------------------------------------------------------------ */
/* Helper: exponent of prime p in n! (Legendre) as signed int64_t */
/* ------------------------------------------------------------ */
static int64_t fact_exp(idx_t n, idx_t p)
{
    int64_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* ------------------------------------------------------------ */
/* Main sequence function */
/* ------------------------------------------------------------ */
seq_t a(idx_t idx)
{
    /* Find block parameter n such that
     * sum_{i=0}^{n-1} i^2 <= idx < sum_{i=0}^{n} i^2  */
    idx_t n = 0;
    idx_t cum = 0;
    while (true) {
        idx_t next = cum + (n + 1) * (n + 1);
        if (next > idx)
            break;
        ++n;
        cum = next;
    }

    idx_t offset      = idx - cum;          /* offset inside this block   */
    idx_t block_size  = n + 1;              /* k and m each range 0..n     */
    idx_t k = offset / block_size;
    idx_t m = offset % block_size;

    /* Prepare primes up to n */
    ensure_primes(n);

    seq_t result = 1;

    /* Multiply by each prime raised to its positive net exponent */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        if (p > n)         /* no further primes needed */
            break;

        int64_t exp = fact_exp(n - m, p)
                    + fact_exp(m, p)
                    - fact_exp(n - k, p)
                    - fact_exp(k, p);

        if (p == 2)
            exp -= (int64_t)n;

        if (exp > 0) {
            seq_t base = (seq_t)p;
            for (int64_t e = 0; e < exp; ++e)
                result *= base;
        }
    }

    return result;
}
