/*
 * A367931  a(n) is the smallest number k such that k*n is an exponentially
 *          odious number (A270428).
 * https://oeis.org/A367931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of sequence A367931
   Formula: a(n) = smallest k such that k*n is exponentially odious.
   For a prime power n = p^e, a(p^e) = p^s(e) where
      s(e) = min{k ≥ e : k is odious} – e.
   Here “odious” means that the number of 1‑bits in the binary
   representation is odd. The function factorizes n, finds the next
   odious exponent for each prime factor, and multiplies the needed
   powers of the primes. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Return true if x has an odd number of 1-bits (odious) */
static bool is_odious(unsigned int x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x) & 1;
#else
    unsigned int c = 0;
    while (x) { c ^= (x & 1); x >>= 1; }
    return c;
#endif
}

/* Smallest integer ≥ e that is odious */
static unsigned int next_odious(unsigned int e)
{
    unsigned int k = e;
    while (!is_odious(k)) ++k;
    return k;
}

/* Main function for sequence A367931 */
seq_t a(idx_t n)
{
    if (n < 2) return 1;          /* a(1) = 1 by definition */

    /* Max number of prime factors for n up to 2^64 is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned int e = (unsigned int)exps[i];
        if (!is_odious(e)) {
            unsigned int k   = next_odious(e);
            unsigned int delta = k - e;
            seq_t p = (seq_t)primes[i];
            for (unsigned int j = 0; j < delta; ++j)
                result *= p;
        }
    }
    return result;
}
