/*
 * A289359  Image of 0 under repeated application of the morphism phi = {x ->
 *          x,x+1,x+2 if x mod 3 = 0; x -> x-1 if x mod 3 = 1; or x -> x+2 if
 *          x mod 3 = 2, for x = 0,1,2,3,...}.
 * https://oeis.org/A289359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289359
 * The sequence is the nth term of the infinite word obtained by
 * iteratively applying the morphism
 *   φ(x) =
 *      x, x+1, x+2   if x mod 3 == 0
 *      x-1           if x mod 3 == 1
 *      x+2           if x mod 3 == 2
 * to the initial word [0].
 * The implementation stores the current iteration in a static cache
 * and expands it lazily until the requested index is available.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of the current iteration */
    static seq_t *word = NULL;
    static size_t wlen = 0;

    /* Initialise with the first word [0] */
    if (wlen == 0) {
        word = (seq_t *)malloc(sizeof(seq_t));
        if (!word) abort();       /* allocation failure */
        word[0] = (seq_t)0;
        wlen   = 1;
    }

    /* Expand until n is within word */
    while (wlen <= (size_t)n) {
        /* Determine length of next word */
        size_t newlen = 0;
        for (size_t i = 0; i < wlen; ++i) {
            seq_t x   = word[i];
            seq_t rem = x % (seq_t)3;
            if (rem == (seq_t)0) newlen += 3;
            else                 newlen += 1;   /* for rem==1 or 2 */
        }

        /* Allocate new word */
        seq_t *newword = (seq_t *)malloc(newlen * sizeof(seq_t));
        if (!newword) abort(); /* allocation failure */

        /* Build next word */
        size_t pos = 0;
        for (size_t i = 0; i < wlen; ++i) {
            seq_t x   = word[i];
            seq_t rem = x % (seq_t)3;
            if (rem == (seq_t)0) {
                newword[pos++] = x;
                newword[pos++] = x + (seq_t)1;
                newword[pos++] = x + (seq_t)2;
            } else if (rem == (seq_t)1) {
                newword[pos++] = x - (seq_t)1;
            } else { /* rem == 2 */
                newword[pos++] = x + (seq_t)2;
            }
        }

        /* Replace old word */
        free(word);
        word = newword;
        wlen = newlen;
    }

    return word[(size_t)n];
}
