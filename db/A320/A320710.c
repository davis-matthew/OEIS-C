/*
 * A320710  Indices of primes followed by a gap (distance to next larger
 *          prime) of 24.
 * https://oeis.org/A320710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): index (1‑based) of the nth prime p_k such that
   p_{k+1} - p_k == 24.  The sequence is obtained by scanning the
   prime list with a sieve.  All results fit in seq_t if they fit in
   seq_t; if seq_t is arbitrary‑precision, the function still works.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for offset 0 */

    /* Static state to allow out‑of‑order calls */
    static seq_t *indices = NULL;     /* array of resulting indices      */
    static size_t idx_len  = 0;        /* number of stored indices        */
    static size_t idx_cap  = 0;        /* current capacity of indices     */
    static uint64_t *primes = NULL;    /* all primes up to current_limit  */
    static size_t prime_cnt  = 0;      /* number of primes in array       */
    static idx_t   last_proc   = 0;    /* next prime index to start from  */
    static idx_t   current_lim  = 0;   /* last limit used for sieve       */

    /* Allocate indices array on first call */
    if (!indices) {
        idx_cap = 128;
        indices = seq_alloc(idx_cap);
    }

    /* Generate more indices until we have at least n of them */
    while (idx_len < (size_t)n) {
        /* Choose next sieve limit — double each time */
        idx_t next_lim = (current_lim == 0) ? 100000
                                            : current_lim * 2 + 10000;

        /* Build primes up to next_lim */
        uint64_t *new_pr = NULL;
        size_t new_cnt   = 0;
        build_primes_upto(next_lim, &new_pr, &new_cnt);

        /* Free previous prime array */
        free(primes);
        primes    = new_pr;
        prime_cnt = new_cnt;

        /* Scan new gaps starting from last_proc */
        for (size_t i = last_proc; i + 1 < prime_cnt; ++i) {
            if (primes[i + 1] - primes[i] == 24) {
                /* Ensure capacity for indices array */
                if (idx_len == idx_cap) {
                    idx_cap *= 2;
                    indices = seq_realloc(indices, idx_cap / 2, idx_cap);
                }
                indices[idx_len++] = (seq_t)(i + 1);  /* 1‑based index */
            }
        }

        last_proc   = prime_cnt - 1;
        current_lim = next_lim;
    }

    return indices[n - 1];
}
