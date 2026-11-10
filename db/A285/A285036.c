/*
 * A285036  Positions of 1 in A285034; complement of A285035.
 * https://oeis.org/A285036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285034, the fixed word of the
 *            morphism 0->10, 1->001 (odd iterates from "0"); computed by
 *            generating the word
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9601 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t len = 1;
    unsigned char *bits = (unsigned char *)malloc(1);
    if (!bits) return (seq_t)0;
    bits[0] = 0;
    int iter = 0;
    size_t ones = 0;
    size_t need = (size_t)n;

    while ((iter & 1) == 0 || ones < need) {
        size_t cap = len * 3;
        unsigned char *nb = (unsigned char *)malloc(cap);
        if (!nb) { free(bits); return (seq_t)0; }
        size_t w = 0;
        ones = 0;
        for (size_t i = 0; i < len; ++i) {
            if (bits[i] == 0) {
                nb[w++] = 1;
                nb[w++] = 0;
                ones += 1;
            } else {
                nb[w++] = 0;
                nb[w++] = 0;
                nb[w++] = 1;
                ones += 1;
            }
        }
        /* recount ones in the new word */
        ones = 0;
        for (size_t i = 0; i < w; ++i) ones += nb[i];
        free(bits);
        bits = nb;
        len = w;
        ++iter;
    }

    size_t cnt = 0;
    size_t pos = 0;
    for (size_t i = 0; i < len; ++i) {
        if (bits[i]) {
            ++cnt;
            if (cnt == need) { pos = i + 1; break; }
        }
    }
    free(bits);
    return (seq_t)pos;
}
