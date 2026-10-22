/*
 * A353086  Column -1 of the extended Trithoff (Tribonacci) array.
 * https://oeis.org/A353086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating the Tribonacci word and counting the
   multiplicity of each non‑negative integer.
   A positive integer m appears twice iff its 1‑based position
   m is a letter "a" in the Tribonacci word.  Zero appears once.  */

#include <stdlib.h>
#include <stdint.h>

static char *trib = NULL;        /* Tribonacci word (0‑based indices) */
static size_t trib_len = 0;      /* current length of trib */

/* Ensure that trib has at least 'need' characters.  */
static void ensure_trib_len(size_t need)
{
    /* Initialise if necessary */
    if (trib_len == 0) {
        trib = (char*)malloc(1);
        trib[0] = 'a';
        trib_len = 1;
    }

    while (trib_len < need) {
        /* Compute length of next word */
        size_t new_len = 0;
        for (size_t i = 0; i < trib_len; ++i) {
            char c = trib[i];
            if (c == 'a' || c == 'b')
                new_len += 2;
            else
                new_len += 1;
        }

        /* Allocate new word */
        char *new_word = (char*)malloc(new_len);
        if (!new_word)
            exit(1);   /* allocation failure */

        /* Build the next word by substitution rules */
        size_t pos = 0;
        for (size_t i = 0; i < trib_len; ++i) {
            char c = trib[i];
            if (c == 'a') {          /* a → ab */
                new_word[pos++] = 'a';
                new_word[pos++] = 'b';
            } else if (c == 'b') {   /* b → ac */
                new_word[pos++] = 'a';
                new_word[pos++] = 'c';
            } else {                  /* c → a */
                new_word[pos++] = 'a';
            }
        }

        free(trib);
        trib = new_word;
        trib_len = new_len;
    }
}

/* Main function computing the n‑th term of the sequence.
   n is 1‑based (a(1) = 0). */
seq_t a(idx_t n)
{
    /* Ensure enough of the Tribonacci word is generated.
       A rough upper bound: twice n is safe. */
    ensure_trib_len((size_t)n * 2u + 1u);

    size_t count = 0;   /* number of terms already produced */
    size_t val   = 0;   /* current integer being considered */

    while (1) {
        if (val == 0) {
            count += 1;              /* zero appears once */
        } else {
            if (trib[val - 1] == 'a')
                count += 2;          /* double if the position is 'a' */
            else
                count += 1;
        }

        if (count >= (size_t)n)
            return (seq_t)val;       /* found the n‑th term */

        ++val;
    }
}
