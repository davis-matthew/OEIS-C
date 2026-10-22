/*
 * A373820  Run-lengths (differing by 0) of antirun-lengths (differing by > 2)
 *          of odd primes.
 * https://oeis.org/A373820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of n-th run of equal antirun‑lengths of odd primes
 *            (i.e. run‑lengths of A027833, the lengths of maximal runs of odd
 *            primes separated by differences > 2)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   generate_odd_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* Standard includes (C11 and C++17 compatible)                  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/generate_odd_primes.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
seq_t a(idx_t n)
{
    /* Static cache of the run‑length values (computed on first call) */
    static seq_t *run_lengths    = NULL;     /* array of run lengths   */
    static idx_t  runlen_count   = 0;        /* number of values stored */
    static bool    initialized   = false;
    static void   (*free_func)() = NULL;      /* cleanup helper */

    /* ---------------------------------------------------------- */
    /* First call – generate primes, compute antirun lengths,     */
    /* and store the run‑length sequence.                          */
    if (!initialized) {
        /* Choose a sieve limit large enough to cover at least 10 000
           desired terms (≈192 000 odd primes).  We use 800 000 to be safe. */
        const idx_t sieve_limit = 800000;

        idx_t *odd_primes = NULL;
        size_t prime_cnt  = 0;
        generate_odd_primes(sieve_limit, &odd_primes, &prime_cnt);
        if (!odd_primes)      /* failure: return 0 (should never happen) */
            return (seq_t)0;

        /* Allocate space for run_lengths (at most prime_cnt values).   */
        run_lengths = (seq_t*)malloc(prime_cnt * sizeof(seq_t));
        if (!run_lengths) {
            free(odd_primes);
            return (seq_t)0;
        }

        /* ----------------------------------------------------- */
        /* Scan odd primes to form antiruns and compute run‑lengths */
        idx_t i = 0;
        idx_t prev_len = 0;   /* 0 means no previous length yet */
        idx_t cur_count = 0;  /* length of current run of equal antirun lengths */

        while (i < prime_cnt) {
            /* Length of the current antirun */
            idx_t run_len = 1;
            idx_t j = i;
            while (j + 1 < prime_cnt &&
                   odd_primes[j + 1] - odd_primes[j] > 2) {
                j++;
                run_len++;
            }

            /* Handle run‑length counting */
            if (prev_len == 0) {          /* first element */
                prev_len  = run_len;
                cur_count = 1;
            } else if (run_len == prev_len) {
                cur_count++;
            } else {
                run_lengths[runlen_count++] = (seq_t)cur_count;
                prev_len   = run_len;
                cur_count  = 1;
            }

            i = j + 1;   /* next antirun starts after current block */
        }
        /* Final run */
        if (prev_len != 0)
            run_lengths[runlen_count++] = (seq_t)cur_count;

        /* Clean up */
        free(odd_primes);

        /* Mark initialization done */
        initialized = true;
    }

    /* ---------------------------------------------------------- */
    /* Return the requested term (sequence is 1‑based)          */
    if (n == 0 || n > runlen_count)
        return (seq_t)0;              /* out of range – should not happen */

    return run_lengths[n - 1];
}
