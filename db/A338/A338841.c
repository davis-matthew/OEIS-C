/*
 * A338841  No odd digit is present in a(n) * a(n+1).
 * https://oeis.org/A338841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 344, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A338841: lexicographically earliest permutation of
   the non‑negative integers such that a(n) · a(n+1) contains no odd digit.
   The algorithm is iterative: having a(n−1), we choose the smallest
   unused integer k for which all decimal digits of a(n−1)·k are even.  */

#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*  Static global state for memoised computation              */
/*------------------------------------------------------------*/
static seq_t *seq        = NULL;   /* seq[1] .. seq[seq_len]  */
static bool  *used       = NULL;   /* used[x] == true if x already appeared */
static size_t  seq_cap   = 0;      /* allocated size of seq  */
static size_t  used_cap  = 0;      /* allocated size of used */
static size_t  seq_len   = 0;      /* number of terms stored (>=0) */

/*------------------------------------------------------------*/
/*  Helper: enlarge seq array if needed                        */
/*------------------------------------------------------------*/
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/*------------------------------------------------------------*/
/*  Helper: enlarge used array if needed                       */
/*------------------------------------------------------------*/
static void ensure_used_cap(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= needed) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    /* initialise new elements to false */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/*------------------------------------------------------------*/
/*  Test whether all decimal digits of x are even             */
/*------------------------------------------------------------*/
static bool all_digits_even(seq_t x)
{
    if (x == (seq_t)0) return true;
    while (x != (seq_t)0) {
        if ((x % 10) & 1) return false;   /* odd digit found */
        x /= 10;
    }
    return true;
}

/*------------------------------------------------------------*/
/*  Main sequence function                                     */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; defensive */

    /* Initialise static tables on first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        used_cap = 16;
        used = (bool *)malloc(used_cap * sizeof(bool));
        for (size_t i = 0; i < used_cap; ++i) used[i] = false;
    }

    /* Ensure capacity for index n and for potential candidate values */
    ensure_seq_cap((size_t)n + 1);
    ensure_used_cap((size_t)n);

    /* Return if already computed */
    if (seq_len >= (size_t)n) return seq[(size_t)n];

    /* Initialise first term if sequence empty */
    if (seq_len == 0) {
        seq[1] = (seq_t)0;          /* a(1) = 0 */
        used[0] = true;
        seq_len = 1;
    }

    /* Build terms up to requested index */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq[i - 1];
        size_t cand = 0;
        while (1) {
            ensure_used_cap(cand + 1);
            if (!used[cand]) {
                seq_t prod = prev * (seq_t)cand;
                if (all_digits_even(prod)) {
                    /* Found the next term */
                    seq[i] = (seq_t)cand;
                    used[cand] = true;
                    break;
                }
            }
            ++cand;
        }
        seq_len = i;
    }

    return seq[(size_t)n];
}
