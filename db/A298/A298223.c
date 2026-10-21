/*
 * A298223  The first of three consecutive primes the sum of which is equal to
 *          the sum of three consecutive squares.
 * https://oeis.org/A298223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   build_primes_upto, extend_capacity_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for the first of three consecutive primes whose
   sum equals the sum of three consecutive squares, building the sequence
   a(n) on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/extend_capacity_seq.h"

/* Integer square root for unsigned __int128.  Returns floor(sqrt(v)). */
static unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= v) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Test whether a triple of consecutive primes satisfies the condition. */
static bool triple_good(uint64_t p, uint64_t q, uint64_t r)
{
    unsigned __int128 sum = (unsigned __int128)p + q + r;
    unsigned __int128 D   = 12 * sum - 24;
    unsigned __int128 t   = isqrt128(D);
    if (t * t != D) return false;
    if ((uint64_t)(t % 6) != 0) return false;
    return true;
}

/* Static storage for primes and the sequence. */
static uint64_t *primes          = NULL;
static size_t    primes_cnt      = 0;
static uint64_t  primes_limit    = 0;

static seq_t *seq                 = NULL;
static size_t  seq_cnt            = 0;
static size_t  seq_cap            = 0;

static size_t  next_triple_idx     = 0;  /* index of first prime for next triple */

/* Ensure primes up to at least 'limit'. Replace existing array. */
static void extend_primes_upto(uint64_t limit)
{
    if (limit <= primes_limit) return;
    uint64_t *newprimes;
    size_t    newcnt;
    build_primes_upto(limit, &newprimes, &newcnt);
    free(primes);
    primes        = newprimes;
    primes_cnt    = newcnt;
    primes_limit  = limit;
}

/* Ensure enough primes for processing the next triple. */
static void ensure_more_primes(size_t needed)
{
    while (primes_cnt < needed) {
        uint64_t new_limit = primes_limit ? primes_limit * 2 + 1000000ULL
                                          : 2000000ULL;
        extend_primes_upto(new_limit);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    /* Initialize prime list once. */
    if (primes_limit == 0) extend_primes_upto(2000000ULL);

    /* Generate terms until we have at least n. */
    while (seq_cnt < (size_t)n) {
        /* Ensure we have enough primes for the next triple. */
        ensure_more_primes(next_triple_idx + 3);

        uint64_t p = primes[next_triple_idx];
        uint64_t q = primes[next_triple_idx + 1];
        uint64_t r = primes[next_triple_idx + 2];

        if (triple_good(p, q, r)) {
            extend_capacity_seq(&seq, &seq_cap, seq_cnt + 1);
            seq[seq_cnt++] = (seq_t)p;
        }
        ++next_triple_idx;
    }

    return seq[n - 1];
}
