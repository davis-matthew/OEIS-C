/*
 * A330946  Odd numbers whose prime indices are not all prime numbers.
 * https://oeis.org/A330946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A330946: odd numbers whose prime indices are not all prime numbers.  */
/*  The sequence is built by scanning odd integers, testing the prime
 *  indices of each, and collecting those that have at least one
 *  composite or “1” index.  It is memoised and works for any order of
 *  calls.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime.h"

static seq_t   *seq_cache = NULL;  /* memoised terms */
static size_t   seq_sz    = 0;
static size_t   seq_cap   = 0;

static uint64_t *primes   = NULL;  /* list of primes discovered so far */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

/* enlarge seq_cache if needed */
static void
ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

/* enlarge primes array if needed */
static void
ensure_prime_cap(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
}

/* append a new prime (assumed greater than last one) */
static void
add_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    primes[prime_cnt++] = p;
}

/* binary search: return 1‑based index of prime p in primes[], or 0 if not found */
static size_t
find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;  /* not found */
}

/* global state: last odd number examined */
static idx_t last_checked = 1;

seq_t a(idx_t n)
{
    /* initialise prime list with 2 */
    if (prime_cnt == 0) {
        ensure_prime_cap(16);
        primes[prime_cnt++] = 2;
    }

    /* generate terms until we have at least n */
    while (seq_sz < (size_t)n) {
        idx_t candidate = last_checked + 2;   /* next odd integer */
        last_checked = candidate;

        uint64_t num = (uint64_t)candidate;
        bool all_prime_indices = true;

        /* trial division by current primes */
        for (size_t i = 0; i < prime_cnt &&
                          (uint64_t)primes[i] * primes[i] <= num; ++i) {
            uint64_t p = primes[i];
            if (num % p == 0) {
                /* prime factor p found, index = i+1 */
                if (!is_prime((idx_t)(i + 1))) {
                    all_prime_indices = false;
                    break;
                }
                while (num % p == 0) num /= p;
            }
        }

        if (all_prime_indices && num > 1) {
            /* remaining factor is prime */
            size_t idx_pos;
            if (num > (prime_cnt ? primes[prime_cnt - 1] : 0)) {
                /* new prime larger than all we know */
                add_prime(num);
                idx_pos = prime_cnt;   /* new prime's index is last */
            } else {
                idx_pos = find_prime_index(num);  /* should exist */
                assert(idx_pos != 0);
            }
            if (!is_prime((idx_t)idx_pos)) all_prime_indices = false;
        }

        if (!all_prime_indices) {
            ensure_seq_cap(seq_sz + 1);
            seq_cache[seq_sz++] = (seq_t)candidate;
        }
    }

    return seq_cache[n - 1];
}
