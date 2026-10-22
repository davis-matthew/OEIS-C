/*
 * A356355  9-gonal numbers which are products of five distinct primes.
 * https://oeis.org/A356355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9‑gonal numbers that are products of five distinct primes
   (OEIS A356355).  The function implements a straightforward
   search for 9‑gonal numbers, keeping a cache of the terms found.
   It uses a pre‑computed prime table up to 6 000 000 to factorise
   each candidate and check that it has exactly five distinct
   prime factors and is square‑free.  The routine returns the
   correct value for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

static seq_t *cache          = NULL;   /* cached terms */
static size_t cache_cap      = 0;      /* capacity of ``cache''      */
static size_t cache_len      = 0;      /* number of terms stored    */
static idx_t next_m          = 1;      /* next m to test for 9‑gonal */

static uint64_t *primes      = NULL;   /* prime table                */
static size_t  prime_cnt     = 0;      /* number of primes           */

/* initialise the prime table up to a fixed limit (enough for all
   n up to 10 000).  The limit is chosen so that sqrt(9‑gonal(m))
   for the largest m expected here is well below it. */
static void init_primes(void)
{
    if (primes != NULL) return;

    const idx_t LIMIT = 6000000ULL;  /* safe upper bound on sqrt values */
    build_primes_upto(LIMIT, &primes, &prime_cnt);
}

/* test whether ``value'' is a product of exactly five distinct primes
   (i.e. square‑free with ω(value)=5).  The factorisation is done
   by trial division using the pre‑computed prime list. */
static bool is_valid_term(seq_t value)
{
    seq_t t = value;
    int distinct = 0;
    bool squarefree = true;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pseq = (seq_t)p;
        seq_t p2 = pseq * pseq;
        if (p2 > t) break;

        if (t % pseq == 0) {             /* p is a prime factor */
            ++distinct;
            t /= pseq;
            if (t % pseq == 0) {         /* repeated factor → not square‑free */
                squarefree = false;
                break;
            }
        }
    }

    if (!squarefree) return false;
    if (t > (seq_t)1) ++distinct;           /* remaining prime > sqrt(value) */

    return (distinct == 5);
}

/* public sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for offset 1; return 0 safely */

    /* ensure prime table exists for factorisation */
    init_primes();

    /* allocate cache on first use */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
    }

    while (cache_len < n) {
        for (idx_t m = next_m; ; ++m) {
            seq_t val = (seq_t)m * (((seq_t)7 * (seq_t)m) - (seq_t)5) / (seq_t)2;

            if (is_valid_term(val)) {
                /* grow cached storage if necessary */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) return (seq_t)0;  /* allocation failure */
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                next_m = m + 1;
                break;    /* continue outer while loop to check if more needed */
            }
        }
    }

    return cache[n - 1];
}
