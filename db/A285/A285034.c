/*
 * A285034  1-limiting word of the morphism 0->10, 1-> 001.
 * https://oeis.org/A285034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A285034 – 1‑limiting word of the morphism 0 → 10, 1 → 001.
 *  a(n) = nth bit (1‑based) of the word obtained after an odd number
 *  of morphism applications, starting from the single symbol "0".
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the sequence, harmless */

    /* start with the 0‑limiting word: "0" (iteration 0 – even) */
    size_t len = 1;
    unsigned char *bits = (unsigned char *)malloc(1);
    if (!bits) return (seq_t)0;    /* allocation failure */
    bits[0] = 0;
    int iter = 0;                  /* number of morphism applications */

    /* keep generating until we have an odd iteration whose length >= n */
    while (len < (size_t)n || (iter & 1) == 0) {
        size_t cap = len * 3;          /* maximum possible length after expansion */
        unsigned char *new_bits = (unsigned char *)malloc(cap);
        if (!new_bits) {
            free(bits);
            return (seq_t)0;          /* allocation failure */
        }

        size_t write = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned char b = bits[i];
            if (b == 0) {              /* 0 → 10 */
                new_bits[write++] = 1;
                new_bits[write++] = 0;
            } else {                    /* 1 → 001 */
                new_bits[write++] = 0;
                new_bits[write++] = 0;
                new_bits[write++] = 1;
            }
        }

        free(bits);
        bits = new_bits;
        len  = write;
        ++iter;
    }

    seq_t result = (seq_t)bits[(size_t)n - 1];
    free(bits);
    return result;
}
