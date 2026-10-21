/*
 * A258853  Third pi-based arithmetic derivative of n.
 * https://oeis.org/A258853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = d_π( d_π( d_π(n) ) ), where the pi‑based arithmetic
 *            derivative d_π is defined by d_π(p) = π(p) for prime p d_π(ab) =
 *            a·d_π(b) + b·d_π(a) for all a,b (π(x) – the prime‑counting
 *            function)
 * Range:     u8 n <= 26, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------  Prime table and π(x) ------------------ */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* The build system supplies these types; they are not overridden here. */
extern seq_t a(idx_t n);  /* forward declaration required by the headers */

/* Upper bound for the sieve – large enough for n ≤ 10000 and
   the values produced by up to three iterations of d_π. */
#define SIEVE_LIMIT 200000U

static uint32_t prime_cnt = 0;
static uint32_t *prime_tbl = NULL; /* list of primes in ascending order   */
static uint32_t *pi_tbl   = NULL;  /* π(x) for 0 ≤ x ≤ SIEVE_LIMIT       */
static int       primes_built = 0;

/* Build sieve of Eratosthenes, prime list and π‑prefix array.  Called once. */
static void build_primes_and_pi(void)
{
    if (primes_built) return;  /* already built */

    /* Allocate arrays */
    prime_tbl = (uint32_t*)malloc((SIEVE_LIMIT / 10 + 1) * sizeof(uint32_t));
    pi_tbl    = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    char *is_comp = (char*)calloc(SIEVE_LIMIT + 1, sizeof(char));
    if (!prime_tbl || !pi_tbl || !is_comp) {
        /* In an unlikely event of allocation failure, just abort. */
        abort();
    }

    /* Sieve */
    uint32_t primes_alloc = SIEVE_LIMIT / 10 + 1;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt >= primes_alloc) {   /* grow array */
                primes_alloc <<= 1;
                prime_tbl = (uint32_t*)realloc(prime_tbl,
                                                primes_alloc * sizeof(uint32_t));
                if (!prime_tbl) abort();
            }
            prime_tbl[prime_cnt++] = i;
            /* Mark multiples, avoid overflow */
            for (uint32_t j = i * 2; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Build π(x) table */
    uint32_t cnt = 0;
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) {
        if (i >= 2 && !is_comp[i]) cnt++;
        pi_tbl[i] = cnt;
    }

    /* Clean up */
    free(is_comp);
    primes_built = 1;
}

/* π(p) for 0 ≤ p ≤ SIEVE_LIMIT is looked up; larger values are uncommon
   for the intended range, but a fallback linear search is provided. */
static inline uint32_t pi_of(uint32_t p)
{
    if (p <= SIEVE_LIMIT) return pi_tbl[p];
    /* Linearly search the prime list for a value ≤ p (rare). */
    for (size_t i = 0; i < prime_cnt; ++i)
        if (prime_tbl[i] > p) return (uint32_t)i;
    return prime_cnt;   /* all primes in list are ≤ p */
}

/* --------------  Pi‑based arithmetic derivative -------------- */

/* Computes d_π(n).  Uses the factorization rule
       d_π(p^e) = e * p^(e-1) * π(p)
   for each prime power and adds the contributions. */
static seq_t der_pi(idx_t n)
{
    if (n <= 1) return 0;

    idx_t orig = n;
    seq_t sum = 0;

    /* Factorization over the precomputed prime list. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];
        if ((idx_t)p * (idx_t)p > n) break;

        if (n % p == 0) {
            /* Count exponent e. */
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }

            /* Compute p^(e-1) as seq_t. */
            seq_t p_pow = 1;
            for (uint32_t k = 1; k < e; ++k) p_pow *= (seq_t)p;

            /* Remaining factor after removing p^e. */
            seq_t rem = (seq_t)(orig / ((idx_t)p * (idx_t)p) * (idx_t)p_pow * (idx_t)p); /* orig/(p^e) */
            /* Simplify: orig / p^e = orig / (p^e) */
            rem = (seq_t)(orig / ((idx_t)p * (idx_t)p)) * (seq_t)p_pow * (seq_t)p; /* avoid integer overflow? */
            /* Actually simpler: rem = orig / (p^e) */
            rem = (seq_t)(orig / ( (idx_t)p_pow * (idx_t)p)); /* p_pow * p = p^e */

            seq_t pi_p = (seq_t)pi_of(p);
            seq_t term = (seq_t)e * p_pow * rem * pi_p;
            sum += term;
        }
    }

    /* If a prime factor remains after dividing by small primes. */
    if (n > 1) {
        uint32_t p = (uint32_t)n;  /* n is < 2^64, fits in uint32_t here for our limits */
        uint32_t e = 1;
        seq_t p_pow   = 1;                /* p^(e-1) = 1 for e=1 */
        seq_t rem     = (seq_t)(orig / p);
        seq_t pi_p    = (seq_t)pi_of(p);
        seq_t term    = (seq_t)e * p_pow * rem * pi_p;
        sum += term;
    }

    return sum;
}

/* ----  Public interface: the sequence a(n) ----------------- */

/* Externally visible function required by the harness. */
seq_t a(idx_t n)
{
    build_primes_and_pi();              /* ensure primes and π table are ready */

    if (n <= 1) return 0;

    seq_t v1 = der_pi(n);
    seq_t v2 = der_pi((idx_t)v1);
    seq_t v3 = der_pi((idx_t)v2);

    return v3;
}
