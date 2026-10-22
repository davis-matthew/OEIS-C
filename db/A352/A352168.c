/*
 * A352168  a(n) = Sum_{p+q = 2n, p<=q, p,q prime} (p*q mod 2n).
 * https://oeis.org/A352168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p+q = 2n, p≤q, p,q prime} (p*q mod 2n)
 * Range:     u8 n <= 16, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve to generate primes up to 2n and
   accumulates the required sum.  It works for any seq_t provided
   (unsigned integer or arbitrary‑precision class). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Sieve data structures                                            */
/* ------------------------------------------------------------------ */
static bool   *sieve_is_prime = NULL;   /* sieve_is_prime[i] == true ⇔ i is prime */
static uint64_t *sieve_primes   = NULL; /* list of primes <= sieve_limit */
static size_t    sieve_prime_cnt = 0;     /* number of primes stored */
static uint64_t  sieve_limit     = 0;       /* maximum number sieved */

/* ------------------------------------------------------------------ */
/* Ensure primes up to 'limit' are available. Grow only when needed.  */
/* ------------------------------------------------------------------ */
static void
ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(sieve_is_prime);
    free(sieve_primes);

    sieve_is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve_is_prime)
        exit(EXIT_FAILURE);

    for (uint64_t i = 2; i <= limit; ++i)
        sieve_is_prime[i] = true;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve_is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve_is_prime[j] = false;
        }
    }

    /* build prime list */
    sieve_prime_cnt = 0;
    sieve_primes = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!sieve_primes)
        exit(EXIT_FAILURE);

    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve_is_prime[i])
            sieve_primes[sieve_prime_cnt++] = i;

    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* The sequence has offset 1; for n < 1 return 0. */
    if (n < 1)
        return (seq_t)0;

    uint64_t two_n = 2 * (uint64_t)n;
    ensure_sieve(two_n);

    seq_t sum = 0;   /* use seq_t to stay compatible with all builds */

    for (size_t idx = 0; idx < sieve_prime_cnt; ++idx) {
        uint64_t p = sieve_primes[idx];
        if (p > n)          /* ensures p ≤ n ≤ q */
            break;

        uint64_t q = two_n - p;
        if (p > q)          /* enforce p ≤ q */
            break;

        if (q <= sieve_limit && sieve_is_prime[q]) {
            seq_t prod = (seq_t)p * (seq_t)q;
            seq_t mod  = prod % (seq_t)two_n;
            sum += mod;
        }
    }

    return sum;
}
