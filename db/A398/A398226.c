/*
 * A398226  Increasing greedy sequence starting with a(1) = 1 whose equally
 *          spaced triples are coprime.
 * https://oeis.org/A398226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: greedy construction of the sequence.
   Starting with a(1)=1, each a(n) is the smallest integer
   greater than a(n‑1) such that every equally spaced triple
   ending at index n is pairwise coprime.  The procedure
   only checks triples where the new term is the last element,
   as all earlier constraints were already satisfied. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0; not used */

    static seq_t *seq  = NULL; /* array holding generated terms   */
    static size_t cap  = 0;    /* allocated capacity               */
    static idx_t   len  = 0;    /* number of terms currently stored */

    /* generate terms up to index n */
    while (len < n) {
        /* next candidate must be larger than the previous term */
        seq_t candidate = (len == 0) ? (seq_t)1 : seq[len-1] + (seq_t)1;

        /* find the first candidate that satisfies all coprime
           conditions for triples ending at the new term */
        for (;;) {
            bool ok = true;
            for (idx_t d = 1; len >= 2*d; ++d) {
                idx_t i = len - 2*d;   /* index of first element of triple */
                idx_t j = len - d;     /* index of second element        */
                seq_t g = gcd(seq[i], seq[j]);     /* gcd(a_i , a_j)   */
                if (gcd(g, candidate) != (seq_t)1) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;
            ++candidate; /* try the next integer */
        }

        /* grow the array if necessary */
        if (len + 1 > cap) {
            size_t new_cap = cap ? cap << 1 : 64;
            while (new_cap <= len + 1) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        seq[len++] = candidate; /* store the accepted term */
    }

    return seq[n-1];
}
