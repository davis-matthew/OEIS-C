/*
 * A276732  Primes p such that (p + 1)/10 is also prime.
 * https://oeis.org/A276732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*A158017(n)-1. A A276732: primes p such that (p+1)/10
 *            is also prime
 * Range:     u8 n <= 4, u16 n <= 146, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

/* ------------------------------------------------------------ */
/* Prime generation ------------------------------------------------ */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t     prime_count = 0;

/* Fill prime_list with all primes up to LIMIT (inclusive).     */
/* LIMIT is chosen large enough for n up to about 10000.         */
static void init_primes_up_to_1M(void)
{
    if (prime_list != NULL)
        return; /* already initialised */

    const size_t LIMIT = 1000000; /* > expected q for n≈10000   */
    bool *is_comp = (bool*)malloc(LIMIT + 1);
    if (!is_comp)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i <= LIMIT; ++i)
        is_comp[i] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* count primes */
    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            ++prime_count;

    prime_list = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i])
            prime_list[pos++] = (uint64_t)i;

    free(is_comp);
}

/* Test primality of n using the pre‑computed prime list.   */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i)
        if (n % prime_list[i] == 0)
            return false;
    return true;
}

/* ------------------------------------------------------------ */
/* Sequence cache ------------------------------------------------ */
/* ------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;
static size_t prime_index = 0; /* next prime q to try */

/* Ensure the cache holds at least n terms (n >= 1). */
static void ensure_terms(idx_t n)
{
    if (n <= (idx_t)seq_len)
        return;

    init_primes_up_to_1M();

    if (seq_cap == 0) {
        seq_cap = 64;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache)
            exit(EXIT_FAILURE);
    }

    while (seq_len < (size_t)n) {
        /* Get next candidate q */
        if (prime_index >= prime_count) {
            /* Should not happen for n up to 10000 with LIMIT = 1M */
            exit(EXIT_FAILURE);
        }
        uint64_t q = prime_list[prime_index++];
        uint64_t p = 10ULL * q - 1ULL;
        if (is_prime_uint64(p)) {
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!seq_cache)
                    exit(EXIT_FAILURE);
            }
            seq_cache[seq_len++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------ */
/* Public function ------------------------------------------------ */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1, a(0) is undefined */
    ensure_terms(n);
    return seq_cache[(size_t)n - 1];
}
