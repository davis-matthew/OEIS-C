/*
 * A276115  Numbers whose digits have a permutation that is a palindrome.
 * https://oeis.org/A276115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 7144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_palindromic_permutation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A276115
   Numbers whose digits have a permutation that is a palindrome.
   Algorithm: sequentially test each integer for the parity condition
   on digit frequencies; memoise results for random access. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_palindromic_permutation.h"

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; index 0 is unused */

    /* Static state for memoisation */
    static seq_t *seq      = NULL;   /* array of found terms          */
    static idx_t  size_cap = 0;      /* allocated capacity           */
    static idx_t  size_len = 0;      /* number of terms stored       */
    static seq_t  cur      = 0;      /* last candidate tested        */

    /* First‑time initialisation */
    if (seq == NULL) {
        size_cap = 16;
        seq = (seq_t *)malloc(size_cap * sizeof(seq_t));
        if (seq == NULL)
            return 0; /* allocation failure: unlikely in test environment */
    }

    /* Find terms until we have at least n of them */
    while (size_len < n) {
        /* generate next candidate */
        cur++;

        /* ensure capacity */
        if (size_len >= size_cap) {
            size_cap <<= 1;
            seq = (seq_t *)realloc(seq, size_cap * sizeof(seq_t));
            if (seq == NULL)
                return 0; /* allocation failure */
        }

        if (has_palindromic_permutation(cur)) {
            seq[size_len++] = cur;
        }
    }

    return seq[n - 1];
}
