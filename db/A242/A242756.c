/*
 * A242756  Semiprimes having only the curved digits.
 * https://oeis.org/A242756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth semiprime whose decimal representation contains only
 *            the curved digits 0,2,3,5,6,8,9 (the sequence A242756)
 * Range:     u8 n <= 27, u16 n <= 2271, u32 n <= 3485, u64 n <= 3485,
 *            u128 n <= 3485, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Characters defining the set of curved digits.
   Bits 0..9 correspond to digits 0..9; a bit set means the digit is allowed. */
static const uint16_t CURVED_DIGIT_MASK =
    (1U << 0) | (1U << 2) | (1U << 3) |
    (1U << 5) | (1U << 6) | (1U << 8) |
    (1U << 9);

/* ---------------------------------------------------------------------- */
/* Test whether the supplied 64‑bit value contains only curved digits.    */
static inline bool has_only_curved_digits(uint64_t x)
{
    if (x == 0) return true;          /* 0 itself is a valid number */
    while (x) {
        uint8_t d = (uint8_t)(x % 10);
        if (!(CURVED_DIGIT_MASK & (1U << d)))
            return false;
        x /= 10;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Prime sieve ---------------------------------------------------------- */
/* We precompute all primes up to a generous limit so that
   the square root of any semiprime we encounter is covered.           */
#define PRIME_SIEVE_LIMIT 10000000UL   /* 10^7  (≈ 0.01 s to sieve)   */
static uint64_t *prime_list   = NULL; /* array of primes               */
static size_t    prime_count = 0;     /* number of primes stored       */
static bool      primes_inited = false;

/* Simple Sieve of Eratosthenes that fills prime_list.               */
static void init_prime_sieve(void)
{
    if (primes_inited) return;
    primes_inited = true;

    /* Allocate a bit array for the sieve. */
    const size_t sieve_size = PRIME_SIEVE_LIMIT + 1;
    unsigned char *sieve = (unsigned char *)malloc(sieve_size);
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sieve_size; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Estimate number of primes to allocate storage. */
    size_t estimated = PRIME_SIEVE_LIMIT / 10;  /* generous overestimate */
    prime_list = (uint64_t *)malloc(estimated * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            if (prime_count >= estimated) {      /* grow storage if needed */
                estimated = estimated * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, estimated * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint64_t)i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    sieve[j] = 0;
            }
        }
    }
    free(sieve);
}

/* ---------------------------------------------------------------------- */
/* Test whether n is a semiprime: it has exactly two prime factors
   (counted with multiplicity).                                         */
static inline bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;     /* 2*2=4 is the smallest semiprime */

    init_prime_sieve();

    uint64_t m = n;
    int factor_cnt = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        while (m % p == 0) {
            ++factor_cnt;
            if (factor_cnt > 2) return false;
            m /= p;
        }
    }
    if (m > 1) {                 /* remaining factor is prime */
        ++factor_cnt;
    }
    return (factor_cnt == 2);
}

/* ---------------------------------------------------------------------- */
/* Cache of previously computed sequence terms.                           */
static seq_t *cache         = NULL;   /* array storing a(1), a(2), …      */
static size_t cache_len     = 0;      /* number of elements stored        */
static size_t cache_cap     = 0;      /* allocated capacity                */
static uint64_t last_checked = 2;     /* last integer examined for prime   */

/* ---------------------------------------------------------------------- */
/* Public sequence function.  It works for any order of calls and
   automatically extends the cache when needed. */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset 1: a(0) is undefined but set to 0           */
        return 0;

    /* Generate and cache terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (!has_only_curved_digits(last_checked))
            continue;
        if (!is_semiprime(last_checked))
            continue;

        /* Ensure sufficient space in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
