/*
 * A260754  a(n) = prime(n+1)! / prime(n).
 * https://oeis.org/A260754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+1)! / prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   factorial, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/factorial.h"    /* seq_t factorial(idx_t n); */

/*  Static state  */
static uint64_t *primes      = NULL;      /* primes[i-1] = prime(i)             */
static size_t   primes_cnt  = 0;          /* number of stored primes           */
static size_t   primes_cap  = 0;          /* allocated capacity                */

static seq_t    cur_fact    = 0;           /* factorial of prime(lastPrimeTop)  */
static size_t   lastComputedTerm = 0;      /* index of last computed a(n)      */

/*  Ensure primes vector holds at least “need” primes  */
static void ensure_primes(size_t need)
{
    while (primes_cnt < need) {
        uint64_t next = 0;
        if (primes_cnt == 0)
            next = next_prime(1);          /* first prime is 2 */
        else
            next = next_prime(primes[primes_cnt - 1]);

        if (primes_cnt >= primes_cap) {
            size_t new_cap = (primes_cap == 0) ? 4 : primes_cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();              /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = next;
    }
}

/*  Compute a(n)  */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;

    size_t ui = (size_t)n;            /* unsigned integer index */

    /* Make sure we have prime(n) and prime(n+1) */
    ensure_primes(ui + 1);

    /* ------------------------------------------------------------ */
    /*  Case 1 : the required term has already been computed last. */
    if (lastComputedTerm == ui)
        return cur_fact / primes[ui - 1];

    /*  Case 2 : we need to extend the current factorial to prime(n+1) */
    if (ui > lastComputedTerm) {
        /* If this is the first call, cur_fact is uninitialised.  */
        if (lastComputedTerm == 0) {
            /* start from scratch */
            cur_fact = factorial(primes[ui]);   /* factorial(prime(n+1)) */
            lastComputedTerm = ui;
            return cur_fact / primes[ui - 1];
        }

        /* Extend cur_fact from factorial(prime(lastComputedTerm+1))
           to factorial(prime(ui+1)) one prime at a time.     */
        for (size_t i = lastComputedTerm + 1; i <= ui; ++i) {
            uint64_t start = primes[i - 1] + 1;       /* prime(i)   + 1   */
            uint64_t end   = primes[i];               /* prime(i+1)       */
            for (uint64_t x = start; x <= end; ++x) {
                cur_fact *= (seq_t)x;
            }
        }
        lastComputedTerm = ui;
        return cur_fact / primes[ui - 1];
    }

    /*  Case 3 : requested n is smaller than previously computed.
        Recompute from scratch.                                */
    cur_fact = factorial(primes[ui]);   /* factorial(prime(n+1)) */
    lastComputedTerm = ui;
    return cur_fact / primes[ui - 1];
}
