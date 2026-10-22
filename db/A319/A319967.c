/*
 * A319967  a(n) = A003145(A003144(n)) where A003144 and A003145 are positions
 *          of '1' and '2' in the tribonacci word A092782.
 * https://oeis.org/A319967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = positions of the word "bac" in the tribonacci * word
 *            obtained as the fixed point of the morphism * a → ab, b → ac, c
 *            → a * (1-based indices, a(1) = 2). The function counts *
 *            occurrences incrementally while generating the word, avoiding *
 *            double-counting by scanning only the newly added par...
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1; n is always ≥1, but guard against 0 for safety */
    if (n == 0)
        return 0;

    /* initial word of the tribonacci word */
    size_t len = 1;
    char *word = (char *)malloc(len);
    if (!word)
        return 0;          /* allocation failure */
    word[0] = 'a';

    idx_t count = 0;        /* number of "bac" seen so far */
    idx_t target = n;       /* we want the nth occurrence */

    while (1)
    {
        /* compute length of the next word */
        size_t newlen = 0;
        for (size_t i = 0; i < len; ++i)
        {
            char ch = word[i];
            if (ch == 'a' || ch == 'b')
                newlen += 2;
            else            /* ch == 'c' */
                newlen += 1;
        }

        /* allocate new word */
        char *newword = (char *)malloc(newlen);
        if (!newword)
        {
            free(word);
            return 0;       /* allocation failure */
        }

        /* generate the next word */
        size_t p = 0;
        for (size_t i = 0; i < len; ++i)
        {
            char ch = word[i];
            if (ch == 'a')
            {
                newword[p++] = 'a';
                newword[p++] = 'b';
            }
            else if (ch == 'b')
            {
                newword[p++] = 'a';
                newword[p++] = 'c';
            }
            else                  /* ch == 'c' */
            {
                newword[p++] = 'a';
            }
        }

        /* scan only the newly added part for new occurrences of "bac" */
        size_t start = (len >= 2) ? (len - 2) : 0;
        for (size_t i = start; i + 2 < newlen; ++i)
        {
            if (newword[i] == 'b' && newword[i + 1] == 'a' && newword[i + 2] == 'c')
            {
                ++count;
                if (count == target)
                {
                    /* position is 1‑based */
                    seq_t pos = (seq_t)(i + 1);
                    free(word);
                    free(newword);
                    return pos;
                }
            }
        }

        /* prepare for next iteration */
        free(word);
        word = newword;
        len = newlen;
    }
}
