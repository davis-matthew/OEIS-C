/*
 * A330560  a(n) = number of primes p <= prime(n) with Delta(p) == 2 (mod 4),
 *          where Delta(p) = nextprime(p) - p.
 * https://oeis.org/A330560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{p ≤ prime(n) : nextprime(p) – p ≡ 2 (mod 4)}|
 * Range:     u8 n <= 418, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a cache of primes and a running cumulative
   count of primes whose prime gap is ≡ 2 (mod 4).  Results are memoised
   so that arbitrary calls for any n ≥ 1 are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"  /* provides: uint64_t next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices – the offset is n = 1. */
    if (n == 0) return 0;

    /* --------------------------------------------------------------------
       Cached data:
       * cum[i]   – a(i), 1 ≤ i ≤ current_max_n
       * primes[j] – the (j+1)-th prime (primes[0] = 2)
       * cum_last_n – the highest n for which cum has been computed
       * primes_cnt – number of primes stored
      -------------------------------------------------------------------- */
    static seq_t *cum = NULL;
    static size_t cum_cap = 0;
    static idx_t cum_last_n = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    /* --------------------------------------------------------------------
       Ensure we have at least (n+1) primes in the primes array:
       we need prime(n) and the next prime prime(n+1).
      -------------------------------------------------------------------- */
    if (primes_cnt < (size_t)n + 1) {
        if (!primes) {
            primes_cap = 128;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
            primes[0] = 2;          /* first prime */
            primes_cnt = 1;
        }
        while (primes_cnt < (size_t)n + 1) {
            uint64_t last = primes[primes_cnt - 1];
            uint64_t p = next_prime(last);
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = p;
        }
    }

    /* --------------------------------------------------------------------
       Grow the cumulative array if necessary.
      -------------------------------------------------------------------- */
    if ((size_t)n + 1 > cum_cap) {
        size_t new_cap = cum_cap ? cum_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        cum_cap = new_cap;
    }

    /* Initialise base case for a(0) = 0. */
    if (cum_last_n == 0) cum[0] = 0;

    /* --------------------------------------------------------------------
       Compute a(k) for all k from cum_last_n+1 up to n.
      -------------------------------------------------------------------- */
    for (idx_t k = cum_last_n + 1; k <= n; ++k) {
        seq_t prev = cum[k - 1];
        uint64_t p_k = primes[k - 1];   /* prime(k)    */
        uint64_t p_next = primes[k];   /* prime(k+1)  */
        uint64_t gap = p_next - p_k;
        if (gap % 4 == 2)
            cum[k] = prev + 1;
        else
            cum[k] = prev;
    }

    cum_last_n = n;
    return cum[n];
}
