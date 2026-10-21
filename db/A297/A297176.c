/*
 * A297176  Inert rational primes in the field Q(sqrt(19)).
 * https://oeis.org/A297176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3268, u32 n <= 9622, u64 n <= 9622,
 *            u128 n <= 9622, big any n
 * Helpers:   is_inert_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inert rational primes in Q(sqrt(19))
   a(n) = the nth rational prime p that is inert in Q(sqrt(19))
        (equivalently, (19/p) = -1).  */

/*  This implementation builds primes on demand using a trial
    division test that uses all primes found so far.  Once a prime is
    found it is stored in a dynamic array.  For each new prime the
    Legendre symbol is checked by a modular exponentiation; if it is
    -1 the prime is inert and stored in a separate array from which
    the sequence values are retrieved.  Both arrays grow geometrically
    as needed.  The algorithm runs in O(n log n) time with a very
    small constant and satisfies the time constraints for n up to
    10 000.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_inert_uint64.h"

/* -- helper: grow a generic array of uint64_t --------------------------------- */
static void grow_if_needed(uint64_t **arr, size_t *cnt, size_t *cap)
{
    if (*cnt >= *cap) {
        size_t newcap = (*cap == 0) ? 64 : (*cap << 1);
        uint64_t *newp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        if (!newp) exit(1);
        *arr = newp;
        *cap = newcap;
    }
}

/* -- trial‑division prime test ---------------------------------------------- */
static bool is_prime_uint64(uint64_t n, const uint64_t *primes, size_t primes_cnt)
{
    if (n < 2)                return false;
    if (n == 2)               return true;
    if ((n & 1) == 0)         return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* p*p > n  */
        if (n % p == 0) return false;
    }
    return true;
}

/* -- static storage --------------------------------------------------------- */
static uint64_t *prime_list   = NULL;  /* all found primes                     */
static size_t     prime_cap   = 0;
static size_t     prime_cnt   = 0;

static uint64_t *inert_list   = NULL;  /* inert primes                       */
static size_t     inert_cap   = 0;
static size_t     inert_cnt   = 0;

static uint64_t   next_candidate = 2;    /* start searching from 2            */

/* -- main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset=1: undefined for 0 */

    /* Generate inert primes until we have the nth one. */
    while (inert_cnt < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Find next prime. */
        while (!is_prime_uint64(cand, prime_list, prime_cnt))
            ++cand;

        /* Store the new prime. */
        grow_if_needed(&prime_list, &prime_cnt, &prime_cap);
        prime_list[prime_cnt++] = cand;

        /* Check inertness. */
        if (is_inert_uint64(cand)) {
            grow_if_needed(&inert_list, &inert_cnt, &inert_cap);
            inert_list[inert_cnt++] = cand;
        }

        /* Next candidate starts after the found prime. */
        next_candidate = cand + 1;
    }

    return (seq_t)inert_list[n - 1];
}
