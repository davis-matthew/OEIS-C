/*
 * A243442  Primes p such that, in base 2, p - digitsum(p) is also a prime.
 * https://oeis.org/A243442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 499, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that p - popcount(p) (binary digit sum) is also prime. */

/*  The implementation uses a single global sieve up to a fixed limit
    (2 000 000) which comfortably covers the first 10 000 terms.
    The list of primes is generated once, and the sequence values
    are produced on demand using a static array that is extended as
    required.  All allocations are performed with malloc/realloc and
    casted appropriately so that the code compiles in both C11 and
    C++17.  No external helper libraries are required. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Global data ---------- */
static const size_t PRECOMPUTE_LIMIT = 2000000;   /* sieve upper bound */

static bool   *sieve      = NULL;   /* prime sieve: true = prime */
static uint32_t *prime_list = NULL; /* list of primes */
static size_t    prime_cnt   = 0;    /* number of primes in prime_list */

/* ---------- Helper: initialise sieve ---------- */
static void init_sieve(void)
{
    if (sieve) return;                 /* already initialised */

    size_t limit = PRECOMPUTE_LIMIT;

    /* Allocate sieve and set all entries to true initially. */
    sieve = (bool*)calloc(limit + 1, sizeof(bool));
    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;        /* 0 and 1 are not prime */

    /* Eratosthenes sieve. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                sieve[j] = false;
            }
        }
    }

    /* Count primes to allocate prime_list. */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) ++prime_cnt;
    }

    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out‑of‑range, caller should not ask for 0 */

    /* static state that persists across calls */
    static size_t processed_idx = 0;          /* next prime to examine */
    static seq_t *results = NULL;             /* stored sequence terms */
    static size_t results_len = 0;            /* number of terms stored */
    static size_t results_cap = 0;            /* capacity of results array */

    /* Initialise sieve and prime list on first call. */
    if (!sieve) init_sieve();

    /* Generate terms until we have at least n of them. */
    while (results_len < (size_t)n) {
        if (processed_idx >= prime_cnt) {
            /* Should not happen for the chosen limit. */
            return (seq_t)0;
        }

        uint32_t p = prime_list[processed_idx++];
        unsigned int pc = __builtin_popcount(p);   /* binary digit sum of p */
        uint32_t q = p - pc;

        if (q >= 2 && sieve[q]) {  /* q is prime as well */
            /* Ensure results array has capacity. */
            if (results_len >= results_cap) {
                size_t new_cap = results_cap ? results_cap * 2 : 32;
                seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
                results = tmp;
                results_cap = new_cap;
            }
            results[results_len++] = (seq_t)p;
        }
    }

    return results[n - 1];   /* sequence is 1‑based */
}
