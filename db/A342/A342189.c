/*
 * A342189  Numbers k such that both k and k+1 are not exponentially
 *          2^n-numbers.
 * https://oeis.org/A342189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342189
   (Numbers k such that both k and k+1 are not exponentially 2^n-numbers)
   a(n) returns the n‑th such number (offset 1). */
/* The function collects candidates until the requested index.
   Exponential 2^n‑numbers are integers whose prime exponents are all powers
   of two.  That test is performed by factorising k with a small prime table.
   The memoised cache allows out‑of‑order calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime table – dynamically grow as needed.                         */
/* ------------------------------------------------------------------ */
static uint32_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Append a new prime to the table. */
static void prime_append(uint32_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 64 : prime_cap << 1;
        uint32_t *tmp = (uint32_t *)realloc(prime_tbl, new_cap * sizeof(uint32_t));
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
    prime_tbl[prime_cnt++] = p;
}

/* Generate prime numbers until the square of the last prime exceeds limit. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) {
        prime_append(2);
    }
    while ((uint64_t)prime_tbl[prime_cnt - 1] * (uint64_t)prime_tbl[prime_cnt - 1] <= limit) {
        uint32_t cand = prime_tbl[prime_cnt - 1] + 1;
        if (prime_cnt > 0 && ((cand & 1) == 0)) cand++;          /* skip even */
        bool ok;
        do {
            ok = true;
            for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= cand; ++i) {
                if (cand % prime_tbl[i] == 0) { ok = false; break; }
            }
            if (!ok) cand += 2;
        } while (!ok);
        prime_append(cand);
    }
}

/* Power‑of‑two test for small exponents. */
static inline bool is_power_of_two(int e)
{
    return e > 0 && (e & (e - 1)) == 0;
}

/* Test if n is an exponentially 2^n‑number (all prime exponents are powers of two). */
static bool is_exponential_2n_uint64(uint64_t n)
{
    if (n <= 1) return true;          /* 1 is vacuously exponential */
    ensure_primes_up_to(n);
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= m; ++i) {
        uint32_t p = prime_tbl[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            if (!is_power_of_two(exp)) return false;
        }
    }
    /* Remaining m, if >1, is a prime with exponent 1 (which is a power of two). */
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache for generated terms – allows out‑of‑order requests.            */
/* ------------------------------------------------------------------ */
static seq_t *cached_terms = NULL;
static size_t terms_len = 0;   /* number of terms stored */
static size_t terms_cap = 0;   /* allocated capacity */
static idx_t next_candidate = 1;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; the sequence is offset 1 */

    /* Lazy initialization of term cache. */
    if (terms_len == 0 && terms_cap == 0) {
        terms_cap = 16;
        cached_terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Grow cache until we have at least n terms. */
    while (terms_len < n) {
        /* Search for next valid k. */
        for (;;) {
            uint64_t k = (uint64_t)next_candidate;
            if (!is_exponential_2n_uint64(k) &&
                !is_exponential_2n_uint64(k + 1)) {
                /* Store term. */
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cached_terms, terms_cap * sizeof(seq_t));
                    cached_terms = tmp;
                }
                cached_terms[terms_len++] = (seq_t)next_candidate;
                ++next_candidate;   /* skip the stored candidate in future searches */
                break;
            }
            ++next_candidate;
        }
    }
    return cached_terms[n - 1];
}
