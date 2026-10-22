/*
 * A309872  For each term, take the last digit of the previous term and count
 *          all the appearances of that digit up to and including the previous
 *          term; the first term is 1.
 * https://oeis.org/A309872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 596, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1. For n>1,
   a(n) = number of occurrences (in the sequence a(1)..a(n-1))
          of the last decimal digit of a(n-1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* --- static state for memoisation --------------------------------- */
    static bool init = false;
    static seq_t *seq = NULL;          /* stored terms, indices 1..cap */
    static size_t cap = 0;             /* highest index computed so far */
    static seq_t digit_totals[10] = {0};/* total counts of digits 0..9 */

    if (n == 0) return 0;              /* undefined for 0 – guard */

    /* -- initialise first term ---------------------------------------- */
    if (!init) {
        /* allocate space for indices 0..1 (ignore 0) */
        seq = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!seq) abort();
        seq[1] = (seq_t)1;
        cap = 1;
        for (int d = 0; d < 10; ++d) digit_totals[d] = 0;
        digit_totals[1] = 1;           /* digit '1' occurs once */
        init = true;
    }

    /* -- answer a request for a previously computed term ------------ */
    if ((size_t)n <= cap) return seq[(size_t)n];

    /* -- grow storage to hold terms up to n ---------------------------- */
    seq_t *tmp = (seq_t *)realloc(seq, ((size_t)n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    size_t old_cap = cap;
    cap = (size_t)n;

    /* -- compute the missing terms from old_cap+1 up to n ----------- */
    for (size_t k = old_cap + 1; k <= cap; ++k) {
        int last_digit = (int)(seq[k - 1] % (seq_t)10);
        seq_t val = digit_totals[last_digit]; /* recurrence value */
        seq[k] = val;

        /* update total digit counts with the digits of val ----------- */
        unsigned char count[10];
        digit_counts(val, count);
        for (int d = 0; d < 10; ++d)
            digit_totals[d] += count[d];
    }

    return seq[(size_t)n];
}
