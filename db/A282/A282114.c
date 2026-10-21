/*
 * A282114  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j+1..k}{(i-j)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 9.
 * https://oeis.org/A282114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_fulcrum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search in base 9: generate numbers that possess a
   fulcrum digit (j, 2 ≤ j ≤ k−1) satisfying
   Σ_{i=j+1..k}(i−j)·d_i = Σ_{i=1..j−1}(j−i)·d_i.
   Cache the first n results so that a(n) is returned in O(1) after
   the required terms have been generated. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_fulcrum.h"

/* Memoised generator of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* array of found terms */
    static size_t sz   = 0;      /* number of terms stored */
    static size_t cap  = 0;      /* capacity of cache   */
    static idx_t  next_candidate = 1;   /* next integer to test */

    if (n == 0)                   /* sequence is defined from 1 upwards */
        return 0;

    /* Generate terms until at least n of them are found. */
    while (sz < n) {
        if (has_fulcrum(next_candidate)) {
            if (sz == cap) {               /* grow cache if needed */
                size_t newcap = (cap == 0) ? 128 : cap << 1;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();                /* unrecoverable */
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)next_candidate;   /* store the term */
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
