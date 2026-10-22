/*
 * A335239  Numbers k such that the k-th composition in standard-order
 *          (A066099) does not have all pairwise coprime parts, where a
 *          singleton is not coprime unless it is (1).
 * https://oeis.org/A335239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the k‑th composition in
 *            standard order (A066099) is NOT pairwise coprime (singletons are
 *            considered not coprime unless they equal 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the required OEIS sequence up to index n.
   The function builds the sequence lazily by generating compositions
   from k=0 upwards until the n­th failure is found. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static state: array of failure positions and current k */
    static seq_t *fails = NULL;
    static size_t cap = 0, len = 0;
    static idx_t current_k = 0;

    /* initialise storage on first invocation */
    if (!fails) {
        cap = 128;
        fails = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!fails) return 0; /* allocation failure – unlikely in tests */
    }

    /* generate until we have enough terms */
    while (len < n) {
        /* k == 0 is always a failure (empty composition) */
        if (current_k == (idx_t)0) {
            if (len == cap) { cap <<= 1; fails = (seq_t *)realloc(fails, cap * sizeof(seq_t)); }
            fails[len++] = (seq_t)0;
            current_k++;
            continue;
        }

        /* build the composition parts from k */
        uint64_t parts[64];
        size_t parts_len = 0;
        idx_t mask = current_k;
        idx_t bitpos = 1;          /* positions are 1‑based for LSB */
        idx_t prev = 0;            /* previous position (0 initially) */
        while (mask) {
            if (mask & (idx_t)1) {
                parts[parts_len++] = (uint64_t)(bitpos - prev);
                prev = bitpos;
            }
            mask >>= 1;
            bitpos++;
            /* guard against exceeding the array – unlikely for needed n */
            if (parts_len >= 64) break;
        }

        /* test pairwise coprimality */
        bool coprime = true;
        if (parts_len == 1) {
            if (parts[0] != 1) coprime = false;
        } else {
            for (size_t i = 0; i < parts_len && coprime; i++) {
                for (size_t j = i + 1; j < parts_len; j++) {
                    if (gcd((seq_t)parts[i], (seq_t)parts[j]) != (seq_t)1) {
                        coprime = false;
                        break;
                    }
                }
            }
        }

        /* if not coprime, record k */
        if (!coprime) {
            if (len == cap) {
                cap <<= 1;
                fails = (seq_t *)realloc(fails, cap * sizeof(seq_t));
            }
            fails[len++] = (seq_t)current_k;
        }

        current_k++; /* advance to next k */
    }

    return fails[n - 1];
}
