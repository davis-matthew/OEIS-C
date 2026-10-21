/*
 * A258030  Numbers prime(k) such that D(prime(k), 3) > 0, where D( * , 3) =
 *          3rd difference.
 * https://oeis.org/A258030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the third difference D(p,3) =
 *            P(k+3) – 3·P(k+2) + 3·P(k+1) – P(k) is positive, where P(k) =
 *            prime(k). The sequence starts with 3,7,13,…
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* ----------  static storage for primes and sequence terms ---------- */
static uint64_t *prime_buf = NULL;   /* generated primes */
static size_t   prime_cnt = 0;       /* number of primes stored   */
static size_t   prime_cap = 0;       /* current capacity          */

static seq_t *term_buf = NULL;       /* primes with D>0           */
static size_t term_cnt  = 0;
static size_t term_cap  = 0;

/* Grow the prime buffer when needed. */
static void grow_prime_buf(void)
{
    size_t new_cap = (prime_cap == 0) ? 64 : (prime_cap << 1);
    uint64_t *tmp = (uint64_t *)realloc(prime_buf, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_buf = tmp;
    prime_cap = new_cap;
}

/* Grow the term buffer when needed. */
static void grow_term_buf(void)
{
    size_t new_cap = (term_cap == 0) ? 64 : (term_cap << 1);
    seq_t *tmp = (seq_t *)realloc(term_buf, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    term_buf = tmp;
    term_cap = new_cap;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined for n≥1 */

    /* Initialise buffers on first use. */
    if (prime_cap == 0) grow_prime_buf();
    if (term_cap  == 0) grow_term_buf();

    /* Generate primes and evaluate the third difference until
       we have at least n qualifying primes. */
    while (term_cnt < (size_t)n) {
        uint64_t p;
        if (prime_cnt == 0) {
            p = 2ULL;                       /* first prime is 2 */
        } else {
            p = next_prime(prime_buf[prime_cnt - 1]);
        }

        /* Append the new prime to the list. */
        if (prime_cnt >= prime_cap) grow_prime_buf();
        prime_buf[prime_cnt++] = p;

        /* A third difference is defined only after we have 4 primes. */
        if (prime_cnt >= 4) {
            uint64_t Pk   = prime_buf[prime_cnt - 4]; /* prime(k)   */
            uint64_t Pk1  = prime_buf[prime_cnt - 3]; /* prime(k+1) */
            uint64_t Pk2  = prime_buf[prime_cnt - 2]; /* prime(k+2) */
            uint64_t Pk3  = prime_buf[prime_cnt - 1]; /* prime(k+3) */

            /* Compare  Pk3 + 3·Pk1  with  3·Pk2 + Pk   using 128‑bit
               arithmetic to avoid overflow. */
            __uint128_t left  = (__uint128_t)Pk3 + (__uint128_t)3 * Pk1;
            __uint128_t right = (__uint128_t)3 * Pk2 + (__uint128_t)Pk;

            if (left > right) {
                /* Pk satisfies the third‑difference condition. */
                if (term_cnt >= term_cap) grow_term_buf();
                term_buf[term_cnt++] = (seq_t)Pk;
            }
        }
    }

    /* Return the requested term (1‑based indexing). */
    return term_buf[n - 1];
}
