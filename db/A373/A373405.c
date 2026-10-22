/*
 * A373405  Sum of the n-th maximal antirun of odd primes differing by more
 *          than two.
 * https://oeis.org/A373405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A373405: a(n) = sum of the n‑th maximal antirun of odd primes
    whose consecutive primes differ by more than two.  The implementation
    generates odd primes, tracks the “runs” where gaps exceed two,
    and memoises the sums so that a(n) can be returned for any n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>

/* The build supplies the scalar integer types seq_t and idx_t.
   Do not #define them here. */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Memoisation data.  All data are lazily initialised on the first call.
       ------------------------------------------------------------------ */
    static bool           initialized  = false;
    static size_t         sums_cap     = 16;         /* capacity of 'sums' */
    static size_t         sums_len     = 0;          /* number of sums stored */
    static seq_t         *sums         = NULL;       /* array of antirun sums */

    static uint64_t      *prime_list   = NULL;       /* primes used for trial division */
    static size_t         prime_cap    = 16;         /* capacity of 'prime_list' */
    static size_t         prime_cnt    = 0;          /* current number of primes       */

    static uint64_t       last_prime   = 0;          /* last prime examined           */
    static seq_t          current_sum  = 0;          /* sum of current antirun         */

    /* ------------------------------------------------------------------
       Initialise on first invocation.  We start with the first odd prime (3).
       ------------------------------------------------------------------ */
    if (!initialized) {
        initialized = true;

        sums       = (seq_t *)malloc(sums_cap * sizeof(seq_t));
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));

        /* prime list begins with 3 */
        prime_list[0] = 3;
        prime_cnt     = 1;

        last_prime    = 3;
        current_sum   = 3;
    }

    /* ------------------------------------------------------------------
       Generate primes and finish antiruns until we have at least n sums.
       ------------------------------------------------------------------ */
    while (sums_len < (size_t)n) {
        /* Find the next odd prime after last_prime.  Candidate increments by 2. */
        uint64_t cand = last_prime + 2;
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            cand += 2;
        }

        /* Decide whether to close the current antirun. */
        uint64_t diff = cand - last_prime;
        if (diff > 2) {                   /* same run → extend */
            current_sum += (seq_t)cand;
        } else {                          /* gap ≤ 2 → close current run */
            /* store the finished run */
            if (sums_len == sums_cap) {
                sums_cap *= 2;
                sums = (seq_t *)realloc(sums, sums_cap * sizeof(seq_t));
            }
            sums[sums_len++] = current_sum;

            /* start a new run beginning with current candidate */
            current_sum = (seq_t)cand;
        }

        /* Make the new prime available for future division tests. */
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = cand;

        last_prime = cand;
    }

    /* the (n-1)th element of sums is the desired value */
    return sums[(size_t)n - 1];
}
