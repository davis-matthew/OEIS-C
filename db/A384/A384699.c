/*
 * A384699  Triples of distinct primes whose sum is a perfect square ordered
 *          by increasing sum and then lexicographically.
 * https://oeis.org/A384699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flatten the triples of distinct primes (p1,p2,p3) with
 *            p1<p2<p3, such that p1+p2+p3 is a perfect square, sorted first
 *            by the square and then lexicographically within each square
 * Range:     u8 n <= 686, u16 n <= 1839, u32 n <= 1839, u64 n <= 1839,
 *            u128 n <= 1839, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   We generate the squares k^2 starting from 4^2=16.
   For each square we iterate over all prime pairs (p,q) with p<q and p+q<k^2,
   compute r = k^2 - p - q, enforce r > q, and test primality of r.
   Whenever a triple is found we output p,q,r sequentially, keeping a running
   counter until we reach the requested index n. Primes are generated on
   demand using the utility function build_primes_upto, and primality is
   tested via is_prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* static storage for primes */
static uint64_t *primes = NULL;
static size_t      primes_cnt = 0;
static uint64_t    primes_limit = 0;

/* ensure we have primes up to at least 'limit' */
static void
ensure_primes(uint64_t limit)
{
    if (limit <= primes_limit)
        return;
    /* free old array if any */
    free(primes);
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    /* build primes up to 'limit' */
    build_primes_upto(limit, &new_primes, &new_cnt);
    primes      = new_primes;
    primes_cnt  = new_cnt;
    primes_limit = limit;
}

/* OEIS sequence A384699 */
seq_t
a(idx_t n)
{
    if (n == 0) /* offset starts at 1, but safeguard */
        return (seq_t)0;

    idx_t pos = 0;   /* number of terms produced so far */
    for (int sqrt_val = 4; ; ++sqrt_val) {          /* start from 4^2 = 16 */
        idx_t sum = (idx_t)sqrt_val * (idx_t)sqrt_val;
        /* need primes up to sum-5 to cover the largest possible r */
        if (sum > 5)
            ensure_primes((uint64_t)(sum - 5));
        else
            ensure_primes(5);  /* at least the first primes */

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p >= (uint64_t)sum)
                break;                                 /* p too large */
            for (size_t j = i + 1; j < primes_cnt; ++j) {
                uint64_t q = primes[j];
                uint64_t pq = p + q;
                if (pq >= (uint64_t)sum)
                    break;                               /* q too large */
                uint64_t r = (uint64_t)sum - pq;
                if (r <= q)             /* must be strictly larger than q */
                    continue;
                /* r must also be less than sum; already ensured by pq < sum */
                if (!is_prime(r))
                    continue;                         /* r not prime */

                /* triple (p, q, r) found; output its primes in order */
                pos++;
                if (pos == n) return (seq_t)p;
                pos++;
                if (pos == n) return (seq_t)q;
                pos++;
                if (pos == n) return (seq_t)r;
            }
        }
    }
}
