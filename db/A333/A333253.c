/*
 * A333253  Lengths of maximal strictly increasing subsequences in the
 *          sequence of prime gaps (A001223).
 * https://oeis.org/A333253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function returns the lengths of maximal strictly increasing
 *            subsequences in the sequence of prime gaps (OEIS A333253). The
 *            implementation generates prime gaps on demand, splits them into
 *            maximal increasing runs, and caches the run lengths so that any
 *            call with n ≥ 1 is answered in O(1) after...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cached lengths of increasing runs */
static seq_t *subseq_lengths = NULL;
static size_t subseq_cap     = 0;
static size_t subseq_count   = 0;        /* number of cached run lengths */

/* Prime list and bookkeeping */
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;        /* number of primes in the list */
static idx_t    prime_limit   = 0;        /* current sieve limit */

/* State of the current run */
static uint64_t last_prime    = 0;        /* last prime processed */
static seq_t    current_len   = 0;        /* length of the current run */
static uint64_t last_gap      = 0;        /* last gap in the current run */
static bool     in_progress   = false;   /* are we inside a run? */
static size_t   next_prime_idx = 0;      /* index of next unused prime */

/* One‑time initialization flag */
static bool initialized = false;

/* Grow the run‑length array to at least new_len elements. */
static void grow_subseq_array(size_t new_len)
{
    if (subseq_cap >= new_len) return;
    size_t new_cap = subseq_cap ? subseq_cap * 2 : 16;
    while (new_cap < new_len) new_cap <<= 1;
    /* realloc is safe for POD types; seq_t is POD for the integer builds
       and for the arbitrary‑precision build the class implements
       trivial construction/destruction. */
    seq_t *tmp = (seq_t *)realloc(subseq_lengths, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    subseq_lengths = tmp;
    subseq_cap     = new_cap;
}

/* Extend the prime sieve and rebuild the prime array. */
static void grow_primes(void)
{
    if (prime_limit == 0) prime_limit = 100;
    else                  prime_limit <<= 1;

    uint64_t *new_primes = NULL;
    size_t    new_cnt     = 0;
    build_primes_upto((idx_t)prime_limit, &new_primes, &new_cnt);

    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = new_cnt;

    /* Adjust next_prime_idx to the first unused prime. */
    size_t i = 0;
    while (i < prime_cnt && prime_list[i] <= last_prime) ++i;
    next_prime_idx = i;
}

/* Ensure that subseq_lengths[0 .. n-1] are computed. */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        grow_primes();                 /* initial sieve */
        last_prime = prime_list[0];
        next_prime_idx = 1;
        initialized = true;
    }

    while (subseq_count < (size_t)n) {
        if (next_prime_idx >= prime_cnt) grow_primes();
        uint64_t next_p = prime_list[next_prime_idx];
        uint64_t gap    = next_p - last_prime;

        if (!in_progress) {
            current_len = (seq_t)1;
            last_gap    = gap;
            in_progress = true;
        } else {
            if (gap > last_gap) {
                ++current_len;
                last_gap = gap;
            } else {
                /* run ends here */
                grow_subseq_array(subseq_count + 1);
                subseq_lengths[subseq_count++] = current_len;
                /* start new run with this gap */
                current_len = (seq_t)1;
                last_gap    = gap;
            }
        }

        last_prime   = next_p;
        ++next_prime_idx;
    }
}

/* External interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset, return 0 */
    ensure_terms(n);
    return subseq_lengths[n - 1];
}
