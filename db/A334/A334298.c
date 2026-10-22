/*
 * A334298  Numbers whose prime signature is a reversed Lyndon word.
 * https://oeis.org/A334298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_rev_lyndon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A334298: numbers whose prime signature is a reversed Lyndon word.
 *  The code computes terms by brute‑force enumeration, storing values in a
 *  cache so that calls in any order are fast.  The implementation keeps
 *  all internal arithmetic in 64‑bit unsigned integers and casts the result
 *  to the generic type `seq_t` on return.  This works for all supported
 *  integer types (uint*_t, unsigned __int128, or an arbitrary‑precision
 *  class).  The program never uses floating‑point or any operations that
 *  would overflow 64 bits.  */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_rev_lyndon.h"
 
/*----- Prime table -----------------------------------------------------*/
#define MAX_PRIME_LIMIT 1000000          /* primes up to 1e6 (~78k primes) */
#define PRIME_CAPACITY 80000            /* enough for 1e6 */

static bool primes_inited = false;
static uint32_t primes[PRIME_CAPACITY];
static size_t prime_cnt = 0;
static bool composite[MAX_PRIME_LIMIT + 1];

/* Initialise the array of primes up to MAX_PRIME_LIMIT by the sieve of
 * Eratosthenes.  The function is called once by a() when needed. */
static void init_primes(void)
{
    if (primes_inited) return;
    /* memset to 0 (false) */
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        composite[i] = false;
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_inited = true;
}

/*----- Cache of computed terms ----------------------------------------*/
static uint64_t *cache_terms = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_checked = 1;     /* next integer to test */

/* Grow the cache array when needed.  The cast is necessary for C++.*/
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        uint64_t *new_ptr = (uint64_t *)realloc(cache_terms,
                                                new_cap * sizeof(uint64_t));
        if (new_ptr) {
            cache_terms = new_ptr;
            cache_cap = new_cap;
        }
    }
}

/*----- Main sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but keep behaviour safe */
    init_primes();

    while (cache_len < (size_t)n) {
        uint64_t num = next_checked++;
        uint64_t tmp = num;

        /* factorise num into exponents */
        int exps[32];
        size_t len = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > tmp) break;
            if (tmp % p == 0) {
                int cnt = 0;
                do {
                    tmp /= p;
                    ++cnt;
                } while (tmp % p == 0);
                exps[len++] = cnt;
            }
        }
        if (tmp > 1) {          /* remaining prime factor */
            exps[len++] = 1;
        }

        if (is_rev_lyndon(exps, len)) {
            grow_cache();
            cache_terms[cache_len++] = num;
        }
    }

    return (seq_t)cache_terms[n - 1];
}
