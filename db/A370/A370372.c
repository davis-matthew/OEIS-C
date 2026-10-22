/*
 * A370372  Row lengths of A369276.
 * https://oeis.org/A370372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = length of the n‑th run of consecutive integers that
 * satisfy the “quality Q” property (neither squarefree nor a prime power),
 * ignoring runs of length 1.  The function scans integers in order,
 * records only runs of length ≥2, and memoises the results so calls need
 * not be in increasing order.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return true if n has at least two distinct prime factors and at least
 * one exponent ≥ 2; i.e., n is neither squarefree nor a prime power. */
static bool is_Q(uint64_t n)
{
    if (n < 12) return false;           /* first Q number is 12 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    if (cnt < 2) return false;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 2) return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* State keeping track of the next integer to examine and the
     * recorded run lengths (only those with length ≥ 2).  */
    static uint64_t curr = 2;
    static idx_t *runs = NULL;
    static size_t used = 0;
    static size_t cap = 0;

    /* Generate runs until we have at least n of them. */
    while (used < (size_t)n) {
        /* Skip non‑Q numbers to the start of the next run. */
        while (!is_Q(curr)) ++curr;

        /* Measure the length of this run. */
        idx_t len = 1;
        ++curr;
        while (is_Q(curr)) {
            ++len;
            ++curr;
        }

        /* Record only runs of length ≥ 2. */
        if (len >= 2) {
            if (used == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(runs, new_cap * sizeof(idx_t));
                if (!tmp) return (seq_t)0;   /* out of memory – abort */
                runs = tmp;
                cap = new_cap;
            }
            runs[used++] = len;
        }
    }

    return (seq_t)runs[n - 1];
}
