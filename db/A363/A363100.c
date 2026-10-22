/*
 * A363100  Fractal sequence which is left unchanged by interleaving it with
 *          the natural numbers, in such a way that each term k of the
 *          sequence is followed by the next k not-yet-seen natural numbers.
 * https://oeis.org/A363100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A363100 – Fractal sequence
 *
 *  Construction:
 *    The sequence is the unique fixed point of the operation
 *    “each term k is followed by the next k natural numbers
 *    that have not yet appeared in the sequence”.
 *    The sequence is built by iteratively expanding a word w:
 *
 *      w' = 1   for initial word w = [1]
 *      w' = F(w)  where
 *          for each x in w (in order)
 *              output x
 *              output the next x smallest unused natural numbers
 *
 *    After each expansion the word grows, and the process
 *    converges to the required sequence.  The value a(n) is the
 *    (n‑1)‑indexed element of that word.
 *
 *  Method:
 *    We keep the current word in a dynamic array 'seq'.
 *    While its length is smaller than the requested index,
 *    we expand it according to the rule above.
 *    The expansion needs to know which natural numbers
 *    have already appeared.  This is handled with a
 *    dynamic bit array 'seen'.
 *
 *  Complexity:
 *    Expansion is linear in the current length, and the
 *    number of expansions needed to reach n=10000 is < 7,
 *    so the routine runs easily in the given time limit.
 *
 *  Note:
 *    The code is valid both for C11 and C++17.  It uses only
 *    standard headers and does not rely on any compiler
 *    extensions.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state across calls */
    static seq_t *seq = NULL;        /* current word */
    static size_t seq_len = 0;       /* current length */

    if (n == 0)        /* safety for out of range index */
        return (seq_t)0;

    while (seq_len < (size_t)n) {
        /* --- expand the current word to the next stage --- */

        /* first expansion: initialise with 1 */
        if (seq_len == 0) {
            seq_len = 1;
            seq = (seq_t *)malloc(sizeof(seq_t));
            seq[0]  = (seq_t)1;
            continue;
        }

        /* compute the sum of all elements (fit in size_t) */
        size_t sum = 0;
        for (size_t i = 0; i < seq_len; ++i)
            sum += (size_t)seq[i];

        size_t new_len = seq_len + sum;
        seq_t *new_seq = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_seq) exit(1);   /* allocation failed */

        /* -----------------  construct 'seen' bit array  ----------------- */
        size_t max_val = 0;
        for (size_t i = 0; i < seq_len; ++i) {
            size_t v = (size_t)seq[i];
            if (v > max_val) max_val = v;
        }

        /* approximate capacity: enough for all numbers that will appear */
        size_t seen_needed = max_val + sum + 4;
        size_t seen_cap   = 64;
        while (seen_cap < seen_needed) seen_cap <<= 1;

        unsigned char *seen = (unsigned char *)calloc(seen_cap, 1);
        if (!seen) { free(new_seq); exit(1); }

        /* ---------------------------------------------------------------
         *  run the transformation: for each element of seq,
         *  output the element, then the next 'value' missing natural numbers.
         * --------------------------------------------------------------- */
        size_t next_missing = 1;      /* next unused natural number */
        size_t pos = 0;              /* position in new_seq */

        for (size_t i = 0; i < seq_len; ++i) {
            seq_t  val_tm = seq[i];
            size_t val    = (size_t)val_tm;

            /* ensure capacity for this value */
            if (val >= seen_cap) {
                while (seen_cap <= val) seen_cap <<= 1;
                seen = (unsigned char *)realloc(seen, seen_cap);
                memset(seen + (seen_cap >> 1), 0, seen_cap - (seen_cap >> 1));
            }
            new_seq[pos++] = val_tm;   /* copy the term itself */
            seen[val] = 1;             /* mark as seen */

            while (next_missing < seen_cap && seen[next_missing])
                ++next_missing;

            /* output the required number of missing naturals */
            for (size_t j = 0; j < val; ++j) {
                while (next_missing < seen_cap && seen[next_missing])
                    ++next_missing;
                if (next_missing >= seen_cap) {          /* grow seen */
                    while (seen_cap <= next_missing) seen_cap <<= 1;
                    seen = (unsigned char *)realloc(seen, seen_cap);
                    memset(seen + (seen_cap >> 1), 0,
                           seen_cap - (seen_cap >> 1));
                }
                new_seq[pos++] = (seq_t)next_missing;   /* output missing */
                seen[next_missing] = 1;                 /* mark seen */
                ++next_missing;
            }
        }

        /* finished one expansion */
        free(seen);
        free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    return seq[n - 1];   /* array is 0‑indexed, OEIS is 1‑indexed */
}
