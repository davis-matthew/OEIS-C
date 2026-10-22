/*
 * A370267  Numbers with an even number of prime factors not of the form 8m+-1
 *          (counting repetitions).
 * https://oeis.org/A370267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   count_non_pm1_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A370267.
   a(n) = the n-th positive integer that has an even number of prime
   factors (counted with multiplicity) that are NOT congruent to 1 or 7
   modulo 8 (i.e., primes ≡ 3 or 5 mod 8, plus the prime 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_non_pm1_factors.h"

/* Cache of computed sequence terms.  seq_cache[0] holds a(1). */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;          /* number of cached terms */
static size_t seq_cap   = 0;          /* allocated capacity */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (seq_len >= n)
        return;

    /* Grow the array if necessary. */
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Generate new terms starting from the integer after the last cached one. */
    seq_t next = (seq_len == 0) ? 1 : seq_cache[seq_len - 1] + 1;
    while (seq_len < n) {
        uint64_t uval = (uint64_t)next;        /* safe for n up to typical limits */
        int fl = count_non_pm1_factors(uval);
        if ((fl & 1) == 0) {                  /* even number → include in sequence */
            seq_cache[seq_len++] = next;
            if (seq_len == seq_cap) {          /* ensure capacity for next insertion */
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
        }
        ++next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)               /* sequence is 1-indexed; n=0 is undefined */
        return 0;
    ensure_cache(n);
    return seq_cache[n - 1];
}
