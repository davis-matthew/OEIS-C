/*
 * A374475  Numbers k such that the least prime dividing the sum of divisors
 *          of k is equal to the least prime not dividing k.
 * https://oeis.org/A374475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3188, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A374475: Numbers k such that the least prime dividing σ(k)
   equals the least prime not dividing k.
   Computed by brute‑force search using a prime sieve. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------*/
/*  Global state for prime sieve and cache                   */
/*------------------------------------------------------------*/
static bool primes_inited = false;
static uint32_t *primes = NULL;   /* array of primes */
static size_t primes_used = 0;    /* number of primes known */

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_size = 0;    /* number of cached terms */
static idx_t next_k = 2;         /* next integer to test (k ≥ 2) */

/*------------------------------------------------------------*/
/*  Build a sieve of Eratosthenes up to LIMIT                 */
/*------------------------------------------------------------*/
static void init_primes(void)
{
    if (primes_inited) return;

    /* Arbitrary but safe upper bound for the needed primes. */
    const idx_t LIMIT = 2000000;         /* 2 000 000 */
    const size_t SIEVE_SIZE = (size_t)(LIMIT + 1);

    bool *is_composite = (bool*)calloc(SIEVE_SIZE, sizeof(bool));
    if (!is_composite) return;  /* memory failure – will leave uninitialised */

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_composite[(size_t)p]) {
            for (idx_t j = p * p; j <= LIMIT; j += p)
                is_composite[(size_t)j] = true;
        }
    }

    /* Allocate initial storage: number of primes ≈ LIMIT/log(LIMIT) */
    size_t capacity = LIMIT / 10 + 10;
    primes = (uint32_t*)malloc(capacity * sizeof(uint32_t));
    if (!primes) { free(is_composite); return; }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[(size_t)i]) {
            if (primes_used == capacity) {
                capacity *= 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, capacity * sizeof(uint32_t));
                if (!tmp) { free(primes); primes = NULL; free(is_composite); return; }
                primes = tmp;
            }
            primes[primes_used++] = (uint32_t)i;
        }
    }

    free(is_composite);
    primes_inited = true;
}

/*------------------------------------------------------------*/
/*  Least prime factor of an arbitrary-precision integer x    */
/*------------------------------------------------------------*/
static seq_t least_prime_factor_seq(seq_t x)
{
    for (size_t i = 0; i < primes_used; ++i) {
        uint32_t p = primes[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > x) break;
        if (x % (seq_t)p == 0) return (seq_t)p;
    }
    /* If no small factor found, x itself is prime (or a product of primes > LIMIT) */
    return x;
}

/*------------------------------------------------------------*/
/*  sigma(k): sum of divisors of k                           */
/*------------------------------------------------------------*/
static seq_t sigma_of_k(idx_t k)
{
    seq_t sigma = 1;
    idx_t temp = k;

    for (size_t i = 0; i < primes_used; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* term = (p^(exp+1) - 1) / (p - 1) */
            seq_t p_pow = 1;
            for (unsigned j = 0; j < exp + 1; ++j) p_pow *= (seq_t)p;
            seq_t term = (p_pow - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
    }

    if (temp > 1) {
        seq_t p = (seq_t)temp;
        sigma *= (p + 1);  /* (p^2-1)/(p-1) = p+1 */
    }

    return sigma;
}

/*------------------------------------------------------------*/
/*  Test whether k satisfies the defining property            */
/*------------------------------------------------------------*/
static bool good_k(idx_t k)
{
    seq_t sigma = sigma_of_k(k);
    seq_t l1 = least_prime_factor_seq(sigma);

    /* first prime not dividing k */
    seq_t l2 = 0;
    for (size_t i = 0; i < primes_used; ++i) {
        uint32_t p = primes[i];
        if (k % p != 0) {
            l2 = (seq_t)p;
            break;
        }
    }
    if (l2 == 0) return false;  /* should not happen as primes cover all numbers */
    return l1 == l2;
}

/*------------------------------------------------------------*/
/*  Return the n-th element of sequence A374475               */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 */

    init_primes();

    /* Grow cache until we have the n-th term. */
    while (cache_size < (size_t)n) {
        /* Find next k satisfying the property. */
        while (!good_k(next_k)) ++next_k;

        /* Append next_k to cache. */
        seq_t *tmp = (seq_t*)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation error – give up */
            free(cache); cache = NULL; cache_size = 0; return 0;
        }
        cache = tmp;
        cache[cache_size++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
