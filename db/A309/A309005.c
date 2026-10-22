/*
 * A309005  Odd squarefree composite numbers m such that m+2 is prime.
 * https://oeis.org/A309005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 4021, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: odd squarefree composite numbers m such that m+2 is prime
   (OEIS A309005).  The function searches odd numbers, skips primes and
   non‑square‑free values, and tests whether m+2 is prime.  Results are
   cached in a static array so that any call for a given n is answered
   in O(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* static cache of sequence values for memoisation */
static seq_t *seq_vals = NULL;
static size_t seq_cap  = 0;
static size_t seq_cnt  = 0;
static idx_t last_tested = 1;   /* start below the first odd candidate */

/* Retrieve the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1; return 0 as a dummy value */

    /* extend the cache until we have at least n terms */
    while (seq_cnt < n) {
        /* move to the next odd number */
        last_tested += 2;
        if (last_tested == 0)       /* wrap‑around guard, not expected */
            break;

        /* skip primes (including 1 which is not prime) */
        if (is_prime(last_tested))
            continue;

        /* m must be composite; check square‑freeness */
        if (!is_squarefree((seq_t)last_tested))
            continue;

        /* require m+2 to be prime */
        if (!is_prime(last_tested + 2))
            continue;

        /* store the found term */
        if (seq_cnt == seq_cap) {
            size_t new_cap = seq_cap == 0 ? 8 : seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* memory allocation failed – abort */
                exit(EXIT_FAILURE);
            }
            seq_vals = tmp;
            seq_cap = new_cap;
        }
        seq_vals[seq_cnt++] = (seq_t)last_tested;
    }

    return seq_vals[n - 1];
}
