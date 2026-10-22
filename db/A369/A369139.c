/*
 * A369139  Numbers k such that Omega(k) = 1 + Omega(k + 1).
 * https://oeis.org/A369139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 8718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369139: k such that Omega(k) = 1 + Omega(k+1) */
#include <stdlib.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    static seq_t *terms = NULL;    /* stored sequence values */
    static size_t len   = 0;        /* number of stored terms */
    static size_t cap   = 0;        /* allocated capacity of terms */
    static idx_t next_k = 1;        /* next integer to test */

    /* Find terms until we reach the n-th element */
    while (len < n)
    {
        /* Count prime factors of k and k+1 */
        seq_t omega_k  = count_prime_factors((seq_t)next_k);
        seq_t omega_k1 = count_prime_factors((seq_t)(next_k + 1));

        /* Condition: Omega(k) = 1 + Omega(k+1) */
        if (omega_k == omega_k1 + 1)
        {
            /* Ensure enough space */
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* allocation failed */
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)next_k;   /* store the term */
        }
        ++next_k;   /* advance to next candidate */
    }

    return terms[n - 1];   /* 1‑based indexing */
}
