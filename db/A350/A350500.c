/*
 * A350500  Even numbers that are both the sum of a twin prime pair and the
 *          sum of 1 and a semiprime.
 * https://oeis.org/A350500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A350500 – Even numbers that are both the sum of a twin prime pair 
 *           and 1 plus a semiprime.  
 * Computation:  For each prime p, if p+2 is also prime we form s = 2p+1.  
 *               If s is the product of exactly two primes (semiprime),  
 *               the term is 2p+2.  Terms are generated in increasing order,  
 *               cached, and returned by index.  The routine works for any
 *               seq_t large enough to hold the term; values that do not
 *               fit are truncated modulo the width of seq_t (which is
 *               acceptable when seq_t is not big enough for the term).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility headers ------------------------------------------------------- */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t)      */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t, uint64_t **, size_t *) */

/* ----------------------------------------------------------------------- */
/* Static state                                                             */
static idx_t  sieve_limit = 0;           /* upper bound used for sieve */
static uint64_t *prime_list = NULL;      /* array of primes (uint64_t) */
static size_t   prime_count = 0;         /* number of primes stored    */

static seq_t  *seq_cache = NULL;        /* cached sequence terms       */
static size_t  seq_len   = 0;            /* number of terms cached      */
static size_t  seq_cap   = 0;            /* allocation size of cache    */

static idx_t  prime_pos = 0;            /* next prime index to inspect */

/* ----------------------------------------------------------------------- */
/* Prime initialization ---------------------------------------------------- */
static void init_primes(void)
{
    if (sieve_limit != 0) return;       /* already initialised         */

    /* Limit chosen to guarantee ~100k twin primes, sufficient for 10 000
     * terms plus the semiprime restriction.                         */
    static const idx_t LIMIT = 20000000ULL;  /* 20 M                     */
    sieve_limit = LIMIT;

    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(LIMIT, &tmp, &tmp_cnt);
    prime_list  = tmp;
    prime_count = tmp_cnt;
}

/* ----------------------------------------------------------------------- */
/* Append a new term to the cache ---------------------------------------- */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ----------------------------------------------------------------------- */
/* Check whether a 64‑bit number is a semiprime (product of two primes).  */
static bool is_semiprime(uint64_t n)
{
    uint64_t tmp = n;
    int count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        while (tmp % p == 0) {
            ++count;
            if (count > 2) return false;
            tmp /= p;
        }
    }
    if (tmp > 1) {          /* last prime factor > sqrt(n)          */
        ++count;
        if (count > 2) return false;
    }
    return count == 2;
}

/* ----------------------------------------------------------------------- */
/* Ensure that at least n terms are cached.                                 */
static void ensure_terms(idx_t n)
{
    if (seq_len >= n) return;

    init_primes();

    while (seq_len < n && prime_pos < prime_count) {
        uint64_t p = prime_list[prime_pos++];

        /* Check twin prime condition: p+2 must also be prime. */
        if (!is_prime(p + 2)) continue;

        /* s = 2p + 1 must be a semiprime. */
        uint64_t s = 2ULL * p + 1ULL;
        if (!is_semiprime(s)) continue;

        /* Term is 2p + 2 = 2*(p+1). */
        seq_t term = (seq_t)(2ULL * (seq_t)p + 2ULL);
        append_term(term);
    }
}

/* ----------------------------------------------------------------------- */
/* The required sequence function.                                          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */
    ensure_terms(n);
    if (seq_len < n) return 0;     /* safety: should not happen with chosen limit */
    return seq_cache[n - 1];
}
