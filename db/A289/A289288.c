/*
 * A289288  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the last digit of a(n-1) plus the
 *          first digit of a(n) is equal to 10. The digit 0 is not allowed.
 * https://oeis.org/A289288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   contains_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289288
   a(1) = 1
   For n>1, a(n) is the smallest unused non‑negative integer (without any digit 0)
   whose first digit f satisfies f + last_digit(a(n-1)) = 10.
   The digit 0 is forbidden in all terms.  */

/* The implementation keeps a memoised vector of already generated terms
   and, for each possible first digit 1..9, the next candidate that must be
   considered when that first digit is required.  Candidates are tested
   for the absence of digit 0 and for being unused by scanning the memoised
   vector.  Complexity is O(n^2) in the worst case; for n≈10000 this
   comfortably fits within the required 2 seconds.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_zero_digit.h"

/* ----------------------------------------------------------------------- */
/*  Check whether a value contains the digit 0.  Works for any unsigned    */
/*  integer type seq_t.                                                   */

/* ----------------------------------------------------------------------- */
/*  Return the n-th term (n>=1).  Calls may arrive in arbitrary order.    */
/*  The sequence is built sequentially up to the largest requested index.  */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static memoisation state -------------------------------------------------- */
    static seq_t *seq      = NULL;   /* vector of computed terms */
    static size_t size     = 0;      /* number of terms stored    */
    static size_t cap      = 0;      /* allocated capacity        */
    static seq_t nextc[10] = {0};    /* next candidate for first digit 1..9 */
    static bool   init_flag = false; /* one‑time initialisation    */

    /* One‑time initialisation: smallest number with given first digit.     */
    if (!init_flag) {
        for (int d = 1; d <= 9; ++d)
            nextc[d] = (seq_t)d;
        init_flag = true;
    }

    /* Ensure n is at least 1 (behaviour for n==0 is undefined).           */
    if (n == (idx_t)0) return (seq_t)0;

    /* Convert n to size_t safely for array bounds.                            */
    size_t target;
    if (n > (idx_t)SIZE_MAX)
        target = SIZE_MAX;      /* pragmatic upper bound */
    else
        target = (size_t)n;

    /* Initialise storage if this is the very first call.                     */
    if (size == 0) {
        cap = 16;
        seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!seq) abort();          /* allocation failure  */
        seq[0] = (seq_t)1;          /* a(1) = 1              */
        size = 1;
    }

    /* Generate terms up to the requested index.                              */
    while (size < target) {
        /* previous term – used to determine required first digit.            */
        seq_t prev = seq[size - 1];
        const seq_t ten = (seq_t)10;
        const seq_t zero = (seq_t)0;

        /* last digit of previous term (1..9). */
        seq_t last_digit = prev % ten;

        /* Required first digit (1..9). */
        int required_first = 10 - (int)last_digit;     /* 1..9 */

        /* Candidate generation loop: find smallest unused number with that   */
        /* first digit and no digit 0.                                         */
        seq_t cand = nextc[required_first];
        for (;;) {
            /* Ensure first digit of cand is the required one.                */
            /* Check by comparing most significant digit.                     */
            seq_t temp = cand;
            while (temp >= ten) temp /= ten;  /* isolate first digit   */
            if ((int)temp != required_first) {
                /* Advance cand to the next number that starts with          */
                /* the required first digit.                                 */
                /* Compute the minimal number with the required first digit   */
                /* that is greater than current cand.                         */
                /* Simple approach: increase cand until condition satisfied.  */
                cand++;
                continue;
            }

            if (contains_zero_digit(cand)) {  /* digit 0 forbidden            */
                cand++;
                continue;
            }

            /* Check if cand is already used.                                 */
            bool used = false;
            for (size_t i = 0; i < size; ++i) {
                if (seq[i] == cand) { used = true; break; }
            }
            if (used) {
                cand++;
                continue;
            }
            break;   /* valid candidate found */
        }

        /* Append the chosen candidate to the sequence.                       */
        if (size + 1 > cap) {
            cap = cap * 2;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            if (!seq) abort();
        }
        seq[size] = cand;
        ++size;

        /* Update next candidate pointer for this first digit.                */
        /* It will start search from cand+1 in the next iteration.           */
        nextc[required_first] = cand + (seq_t)1;
    }

    /* Return the requested term.                                            */
    return seq[target - 1];
}
