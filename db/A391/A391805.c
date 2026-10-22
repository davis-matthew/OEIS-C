/*
 * A391805  a(n) = greatest prime less than prime(n)*prime(n+1).
 * https://oeis.org/A391805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime < prime(n)*prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------ */
/*  Prime table utilities – small sieve up to SIEVE_LIMIT                     */
/* ------------------------------------------------------------------------ */
#define SIEVE_LIMIT 200000  /* sufficient for n ≤ 10 000 (prime ~104 729) */

static unsigned char *isprime_flag = NULL;  /* 0/1 sieve flags */
static uint32_t     *prime_list    = NULL;  /* array of primes up to SIEVE_LIMIT */
static size_t        prime_count   = 0;       /* number of primes found */

/* Build the sieve and prime list once */
static void build_prime_table(void)
{
    if (prime_count) return; /* already built */

    isprime_flag = (unsigned char*)calloc(SIEVE_LIMIT + 1, 1);
    if (!isprime_flag) return; /* allocation failure – caller will return 0 */

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) isprime_flag[i] = 1;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (isprime_flag[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) isprime_flag[j] = 0;
        }
    }

    /* Count primes */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) if (isprime_flag[i]) prime_count++;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) return; /* allocation failure */

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) if (isprime_flag[i])
        prime_list[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------------------ */
/*  Primality test using the precomputed primes (trial division).          */
/* ------------------------------------------------------------------------ */
static bool is_prime_seq(seq_t x)
{
    if (x <= 1) return false;
    if (x == 2) return true;
    if ((x % (seq_t)2) == 0) return false;

    if ((idx_t)x <= SIEVE_LIMIT) {
        return isprime_flag[(size_t)x];
    }

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/*  Main sequence function.                                               */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, but return 0 safely */
    build_prime_table();

    /* Ensure we have at least n+1 primes; our sieve covers n <= 10 000 */
    if ((size_t)n + 1 > prime_count) return 0;  /* safety check */

    seq_t p1 = (seq_t)prime_list[n - 1];
    seq_t p2 = (seq_t)prime_list[n];          /* prime(n+1) */
    seq_t prod = p1 * p2;

    seq_t cand = prod - 1;                     /* start just below product */

    /* Search downward for the greatest prime less than prod */
    for (;; --cand) {
        if (cand < 2) /* should never happen for valid n */
            return 2;
        if (is_prime_seq(cand)) return cand;
    }
}
