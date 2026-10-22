/*
 * A362225  Primes of the form (2*p^2 + 1)/3 where p is a prime > 3.
 * https://oeis.org/A362225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th prime of the form (2*p^2+1)/3
 *  where p is a prime > 3.
 *  It builds a prime list on demand and caches the sequence values.
 *  The implementation uses only integer arithmetic and the provided
 *  generic primality routine is_prime_big.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* ---------- Prime cache ------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* primes found so far, starting 2,3 */
static size_t    prime_len   = 0;
static size_t    prime_cap   = 0;

/* Ensure capacity for at least n primes in prime_cache. */
static void ensure_prime_cap(size_t n)
{
    if (n <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < n) new_cap = n;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Return the next prime > 3; primes 2 and 3 are stored first. */
static uint64_t next_prime(void)
{
    static uint64_t candidate = 5;       /* candidate odd number after 3 */

    /* Ensure the prime cache is initialized. */
    if (prime_len == 0) {
        ensure_prime_cap(2);
        prime_cache[0] = 2;
        prime_cache[1] = 3;
        prime_len = 2;
    }

    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_cache[i];
            /* stop when p^2 > candidate to avoid overflow */
            if ((unsigned __int128)p * p > candidate) break;
            if (candidate % p == 0) { ok = false; break; }
        }
        if (ok) {
            /* cache the new prime and advance candidate */
            ensure_prime_cap(prime_len + 1);
            prime_cache[prime_len++] = candidate;
            uint64_t res = candidate;
            candidate += 2;   /* skip even numbers */
            return res;
        }
        candidate += 2;
    }
}

/* ---------- Sequence cache --------------------------------------------- */
static seq_t *seq_cache = NULL;   /* stored sequence values */
static size_t seq_len   = 0;       /* number of values stored */
static size_t seq_cap   = 0;

/* Ensure capacity for at least n sequence values. */
static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe return */

    /* Generate terms until we have at least n cached. */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime();                /* p > 3 automatically */
        seq_t val = ((seq_t)2 * (seq_t)p * (seq_t)p + 1) / 3;
        if (is_prime_big(val)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = val;
        }
    }
    return seq_cache[n - 1];
}
