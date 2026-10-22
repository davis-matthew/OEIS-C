/*
 * A350174  For k = 0, 1, 2, 3, ... write k prime(k+1) times.
 * https://oeis.org/A350174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest k such that sum of first k primes <= n
 * Range:     u8 n <= 128, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = A083375(n+1) - 1 with memoized prime list. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: prime list and cumulative sums */
    static uint64_t *primes   = NULL;  /* primes[0..prime_cnt-1] */
    static seq_t   *cum      = NULL;  /* cum[i] = sum of first (i+1) primes */
    static size_t  prime_cnt = 0;     /* number of primes stored   */
    static size_t  prime_cap = 0;     /* allocation capacity       */
    static uint64_t last_prime = 1;   /* last prime found (initially 1) */

    /* cast index once (seq_t may be larger type) */
    seq_t nn = (seq_t)n;

    /* Grow primes/cumulative sums until the last cumulative sum exceeds n */
    while (prime_cnt == 0 || cum[prime_cnt-1] <= nn)
    {
        /* Find the next prime after last_prime */
        uint64_t cand = last_prime + 1;
        for (;;)
        {
            bool ok = true;
            for (size_t i = 0; i < prime_cnt; ++i)
            {
                uint64_t p = primes[i];
                if (p > cand / p) break;     /* p*p may overflow */
                if (cand % p == 0)
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                last_prime = cand;

                /* Grow storage if needed */
                if (prime_cnt == prime_cap)
                {
                    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                    uint64_t *p_t  = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    seq_t   *c_t  = (seq_t *)realloc(cum,    new_cap * sizeof(seq_t));
                    primes = p_t;
                    cum    = c_t;
                    prime_cap = new_cap;
                }

                /* Store prime and update cumulative sum */
                primes[prime_cnt] = cand;
                if (prime_cnt == 0)
                    cum[prime_cnt] = (seq_t)cand;
                else
                    cum[prime_cnt] = cum[prime_cnt-1] + (seq_t)cand;

                ++prime_cnt;
                break;   /* next prime found, exit inner loop */
            }
            ++cand;
        }
    }

    /* After the loop, cum[prime_cnt-1] > n, so a(n) = prime_cnt-1 */
    size_t k = prime_cnt - 1;
    return (seq_t)k;
}
