/*
 * A369854  a(n) = number of nonempty subsets S of {1,2,...,n} such that
 *          (number of nonprimes in S) = (number of primes in S).
 * https://oeis.org/A369854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, pi(n)) - 1 (pi(n) = number of primes ≤ n)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 39, u64 n <= 78, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ======================================================================== */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------  Sieve data  --------------------------------------------- */
static idx_t   sieve_limit   = 0;          /* largest n that has been sieved   */
static uint8_t *prime_flags  = NULL;       /* 0 = composite, 1 = prime         */
static idx_t   *pi_prefix    = NULL;       /* pi_prefix[i] = π(i)               */
static uint64_t *prime_list  = NULL;       /* list of primes <= sieve_limit    */
static size_t   prime_cnt    = 0;           /* number of primes found           */

/* ----------  Utility to sieve up to at least n  ----------------------- */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;   /* already sieved up to required limit */

    size_t new_len = (size_t)(n + 1);

    /* Allocate or resize prime_flags */
    prime_flags = (uint8_t *)realloc(prime_flags, new_len * sizeof(uint8_t));
    if (prime_flags == NULL) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)n; ++i)
        prime_flags[i] = 1;
    if (n >= 0) prime_flags[0] = 0;
    if (n >= 1) prime_flags[1] = 0;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (prime_flags[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                prime_flags[m] = 0;
        }
    }

    /* Resize prime_list to hold all primes up to n */
    prime_list = (uint64_t *)realloc(prime_list, new_len * sizeof(uint64_t));
    if (prime_list == NULL) exit(EXIT_FAILURE);

    /* Build pi_prefix and prime_list */
    pi_prefix = (idx_t *)realloc(pi_prefix, new_len * sizeof(idx_t));
    if (pi_prefix == NULL) exit(EXIT_FAILURE);
    size_t pc = 0;
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (prime_flags[i]) {
            ++pc;
            prime_list[pc - 1] = (uint64_t)i;
        }
        pi_prefix[i] = pc;
    }
    prime_cnt = pc;
    sieve_limit = n;
}

/* ----------  Exponent of prime p in N!  ----------------------------- */
static uint64_t exp_in_factorial(idx_t N, uint64_t p)
{
    uint64_t exp = 0;
    if (N == 0)
        return 0;
    uint64_t div = p;
    while (div <= (uint64_t)N) {
        exp += N / div;
        /* avoid overflow on next multiplication */
        if (div > (uint64_t)N / p)
            break;
        div *= p;
    }
    return exp;
}

/* ----------  Main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* a(0) and a(1) are zero */

    ensure_sieve(n);                 /* makes sure we have π(n) and primes */

    idx_t  pi_n = pi_prefix[n];      /* π(n) */
    if (pi_n == 0)  /* n < 2, handled above, but keep for safety */
        return 0;

    seq_t res = 1;   /* binomial coefficient */

    /* Iterate over all primes up to n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        /* compute exponent of p in C(n,π(n)) */
        uint64_t e = exp_in_factorial(n, p);
        e -= exp_in_factorial((idx_t)pi_n, p);
        e -= exp_in_factorial(n - pi_n, p);

        /* multiply res by p^e (done multiplicatively to avoid overflow) */
        for (uint64_t k = 0; k < e; ++k)
            res *= (seq_t)p;
    }

    /* exclude the empty subset */
    return res - (seq_t)1;
}
