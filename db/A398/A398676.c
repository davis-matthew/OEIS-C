/*
 * A398676  Numbers k such that sigma(3*k+2) > sigma(3*k), where sigma(k) is
 *          the sum of divisors of k (A000203).
 * https://oeis.org/A398676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9775 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer k such that σ(3k+2) > σ(3k).  The function
   builds the sequence on demand and memoises it across calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is 1‑based */

    /* memoised data */
    static seq_t *terms = NULL;         /* array of found k's          */
    static size_t cap   = 0;            /* allocated capacity          */
    static size_t sz    = 0;            /* number of terms stored      */
    static idx_t   next_k = 1;          /* next k to test              */

    /* initialise storage if first call */
    if (terms == NULL) {
        cap = 16;
        terms = mem_new(seq_t, cap);
        if (!terms) exit(EXIT_FAILURE);
    }

    /* extend the table until we have at least n terms */
    while (sz < n) {
        idx_t k      = next_k;
        idx_t a3k    = 3 * k;
        idx_t a3k2   = a3k + 2;
        seq_t s3k    = sigma_of(a3k);
        seq_t s3k2   = sigma_of(a3k2);
        if (s3k2 > s3k) {
            if (sz >= cap) {            /* double capacity if needed  */
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, terms, cap);
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
            }
            terms[sz++] = (seq_t)k;
        }
        next_k++;                       /* advance to next candidate  */
    }

    return terms[n-1];
}
