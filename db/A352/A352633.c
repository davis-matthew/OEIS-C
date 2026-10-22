/*
 * A352633  Lexicographically earliest sequence of distinct positive integers
 *          such for any n > 0, a(n) and a(n+1) are coprime and have no common
 *          1-bits in their binary expansions.
 * https://oeis.org/A352633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352633:  lexicographically earliest sequence of distinct positive
   integers such that for all n > 0, a(n) and a(n+1) are coprime and have no
   common 1‑bits in their binary expansions.  The parity of a(n) equals the
   parity of n.  a(1)=1.  For n ≥ 2, a(n) is the smallest unused positive
   integer of the required parity that satisfies gcd(a(n-1),a(n))=1 and
   (a(n-1) & a(n))==0.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, harmless */

    /* Static storage for the sequence and the “used” flag table. */
    static seq_t *seq = NULL;
    static size_t seq_size = 0, seq_cap = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;

    /* Grow the sequence until it contains at least n terms. */
    while (seq_size < n) {
        /* Ensure capacity for the sequence array. */
        if (seq_size == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!seq)
                return (seq_t)0;   /* out‑of‑memory: abort */
            seq_cap = new_cap;
        }

        /* First term a(1) = 1. */
        if (seq_size == 0) {
            seq[0] = (seq_t)1;

            /* Ensure the used‑flag array can index 1. */
            if (used_cap <= 1) {
                size_t new_used = used_cap ? used_cap * 2 : 2;
                used = (bool *)realloc(used, new_used * sizeof(bool));
                if (!used)
                    return (seq_t)0;
                for (size_t i = used_cap; i < new_used; ++i)
                    used[i] = false;
                used_cap = new_used;
            }
            used[1] = true;
            seq_size = 1;
            continue;
        }

        /* Current state before choosing a(n). */
        seq_t prev = seq[seq_size - 1];
        size_t next_index = seq_size + 1;                /* 1‑based index of new term */
        seq_t cand = ((next_index & 1U) ? (seq_t)1 : (seq_t)2);   /* parity of a(n) */

        /* Find the smallest candidate satisfying the rules. */
        for (;;) {
            /* Make sure the used‑flag array covers the candidate. */
            if ((size_t)cand >= used_cap) {
                size_t new_used = used_cap ? used_cap * 2 : 2;
                while (new_used <= (size_t)cand)
                    new_used <<= 1;
                used = (bool *)realloc(used, new_used * sizeof(bool));
                if (!used)
                    return (seq_t)0;
                for (size_t i = used_cap; i < new_used; ++i)
                    used[i] = false;
                used_cap = new_used;
            }

            if (!used[(size_t)cand] &&
                gcd(prev, cand) == (seq_t)1 &&
                ((prev & cand) == (seq_t)0))
                break;             /* Valid candidate found */

            cand += (seq_t)2;    /* Keep the required parity. */
        }

        /* Store the chosen term and mark it as used. */
        seq[seq_size] = cand;
        seq_size++;
        used[(size_t)cand] = true;
    }

    return seq[n - 1];
}
