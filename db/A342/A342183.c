/*
 * A342183  Primes q such that 15*q-2 and 15*q+2 are cousin primes.
 * https://oeis.org/A342183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime q such that 15*q-2 and 15*q+2 are both primes.
   Computed by successive prime testing (no closed‑form formula). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_cache = NULL;     /* stored a(k) values               */
static idx_t seq_len   = 0;          /* number of values stored so far  */
static idx_t seq_cap   = 0;          /* allocated capacity of seq_cache*/
static idx_t next_candidate = 2;     /* next integer to test for q      */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n <= seq_len)
        return;

    while (seq_len < n)
    {
        /* Find the next prime q starting from next_candidate. */
        idx_t q = next_candidate;
        while (!is_prime_big((seq_t)q) && q != 0)   /* uint64_t wrap guard */
            ++q;
        if (q == 0)                                /* overflow, stop */
            break;
        next_candidate = q + 1;

        /* Compute 15*q ± 2. */
        seq_t q_t   = q;                /* convert to seq_t */
        seq_t v15q  = q_t * 15;         /* 15*q   */
        seq_t cand1 = v15q - 2;         /* 15*q-2 */
        seq_t cand2 = v15q + 2;         /* 15*q+2 */

        if (is_prime_big(cand1) && is_prime_big(cand2))
        {
            /* Grow the cache if necessary. */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq_cache = tmp;
            }
            seq_cache[seq_len++] = q_t;   /* store the qualifying prime q */
        }
    }
}

/* External entry point demanded by the checker */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined below offset */
    ensure_cache(n);
    return seq_cache[n - 1];
}
