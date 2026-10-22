/*
 * A367479  a(n) is the number of steps required for prime(n) to reach 2 when
 *          iterating the following hailstone map: If P == 5 (mod 6), then P
 *          -> next_prime(P + ceiling(sqrt(P))), otherwise P ->
 *          previous_prime(ceiling(sqrt(P))); or a(n) = -1 if prime(n) never
 *          reaches 2.
 * https://oeis.org/A367479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of steps required for prime(n) to reach 2
   using the hailstone map:
      if P % 6 == 5  ->  P = next_prime(P + ceil(sqrt(P)))
      otherwise      ->  P = prev_prime( ceil(sqrt(P)) )
   The function implements this map iteratively. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* Find the smallest prime ≥ start (linear search). */
static inline uint64_t next_prime_uint64(uint64_t start)
{
    for (uint64_t p = start;; ++p) {
        if (is_prime(p)) return p;
    }
}

/* Find the largest prime ≤ start (linear search). */
static inline uint64_t prev_prime_uint64(uint64_t start)
{
    for (uint64_t p = start;; --p) {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* offset 1: a(1)=0 for prime 2 */
    if (n < 1) return 0;

    /* starting prime */
    uint64_t p = (uint64_t)get_prime(n);
    seq_t steps = 0;

    while (p != 2) {
        /* ceil(sqrt(p)) */
        seq_t fsq_seq = floor_sqrt((seq_t)p);
        uint64_t fsq = (uint64_t)fsq_seq;       /* floor sqrt */
        uint64_t ceil_sqrt = fsq;
        if (ceil_sqrt * ceil_sqrt < p) ++ceil_sqrt;

        if ((p % 6ull) == 5ull) {
            /* next_prime(P + ceil_sqrt) */
            p = next_prime_uint64(p + ceil_sqrt);
        } else {
            /* prev_prime(ceil_sqrt) */
            p = prev_prime_uint64(ceil_sqrt);
        }
        ++steps;
    }

    return steps;
}
