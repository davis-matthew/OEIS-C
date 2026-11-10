/*
 * A285566  Positions of 0 in A285565; complement of A285567.
 * https://oeis.org/A285566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285565, the fixed point of
 *            0->01100110, 1->110110011011 (the square of 0->11, 1->0110)
 *            starting from 0; computed by iterating the morphism
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9724 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static unsigned char *word = NULL;
    static size_t len = 0;
    static uint64_t *pos = NULL;
    static size_t zc = 0;

    if (len == 0)
    {
        word = (unsigned char *)malloc(1);
        word[0] = 0;
        len = 1;
        pos = (uint64_t *)malloc(sizeof(uint64_t));
        pos[0] = 1;
        zc = 1;
    }

    static const unsigned char b0[8]  = {0,1,1,0,0,1,1,0};
    static const unsigned char b1[12] = {1,1,0,1,1,0,0,1,1,0,1,1};

    while ((uint64_t)zc < (uint64_t)n)
    {
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (word[i] == 0) ++zeros; else ++ones;
        }
        size_t newLen = zeros * 8 + ones * 12;
        unsigned char *nw = (unsigned char *)malloc(newLen);
        size_t p = 0;
        for (size_t i = 0; i < len; ++i)
        {
            if (word[i] == 0)
            {
                for (size_t j = 0; j < 8; ++j) nw[p++] = b0[j];
            }
            else
            {
                for (size_t j = 0; j < 12; ++j) nw[p++] = b1[j];
            }
        }
        free(word);
        word = nw;
        len = newLen;

        size_t cnt = 0;
        for (size_t i = 0; i < len; ++i)
            if (word[i] == 0) ++cnt;
        free(pos);
        pos = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        size_t k = 0;
        for (size_t i = 0; i < len; ++i)
            if (word[i] == 0) pos[k++] = (uint64_t)i + 1;
        zc = cnt;
    }

    return (seq_t)pos[(size_t)(n - 1)];
}
