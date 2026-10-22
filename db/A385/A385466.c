/*
 * A385466  Primes that are at the end of the local maxima in the sequence of
 *          consecutive prime gaps.
 * https://oeis.org/A385466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385466: primes at the end of local maxima in the sequence of consecutive prime gaps.
   Computation: iterate over primes, track gaps g(k) = p(k+1)-p(k).
   When for some k we have g(k-1) < g(k) > g(k+1), then p(k+1) is recorded
   (the larger prime of the local‑maximum gap). */

/* The sequence is indexed from 1; a(1) is the first value listed in the OEIS entry. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"            /* provides uint64_t next_prime(uint64_t start) */

#define MAX_CACHE 20000                      /* sufficient for n up to ~10000 */

static seq_t  cache[MAX_CACHE];
static size_t len        = 0;               /* number of terms already generated */

/* state used while generating new terms */
static uint64_t  last_prime          = 0;
static uint64_t  last_gap1           = 0; /* gap between last_prime and previous prime */
static uint64_t  last_gap2           = 0; /* gap before last_gap1 */
static uint64_t  prime_of_last_gap1  = 0; /* larger prime of last_gap1 */
static uint64_t  gaps_cnt            = 0;  /* number of gaps seen so far */
static bool      init                = false;

static void init_state(void)
{
    if (init) return;
    init = true;
    len = 0;
    last_prime = 2ULL;          /* start from the first prime */
    last_gap1  = 0;
    last_gap2  = 0;
    prime_of_last_gap1 = 0;
    gaps_cnt = 0;
}

/* Return the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    init_state();

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        /* Find the next prime after the current one. */
        uint64_t p = next_prime(last_prime);

        /* New gap between the last prime and the one we just found. */
        uint64_t g_new = p - last_prime;

        /* When we have at least two previous gaps, test if the middle
           one is a local maximum: gap2 < gap1 > g_new. */
        if (gaps_cnt >= 2)
        {
            if (last_gap2 < last_gap1 && last_gap1 > g_new)
            {
                if (len < MAX_CACHE)
                {
                    cache[len++] = (seq_t)prime_of_last_gap1;
                }
                else
                {
                    /* In practice this should never happen for n <= 10000. */
                    /* If it does, we silently stop generating more terms. */
                    break;
                }
            }
        }

        /* Shift gap history for the next iteration. */
        last_gap2 = last_gap1;
        last_gap1 = g_new;
        prime_of_last_gap1 = p;
        gaps_cnt++;

        /* Update last_prime to the newly generated prime. */
        last_prime = p;
    }

    /* After the loop we are guaranteed len >= n (or we halted
       because the cache is full).  Return the cached term. */
    return cache[(size_t)(n - 1)];
}
