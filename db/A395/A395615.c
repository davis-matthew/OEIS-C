/*
 * A395615  a(n) is the integer obtained by inserting the product (modulo 10)
 *          of adjacent digits of n between them.
 * https://oeis.org/A395615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) is formed by inserting (digit_i * digit_{i+1}) mod 10
 *            between each pair of adjacent decimal digits of n. Single‑digit
 *            n remain unchanged
 * Range:     u8 n <= 22, u16 n <= 609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* single‑digit numbers are unchanged */
    if (n < 10)
        return (seq_t)n;

    uint64_t tmp = n;
    uint8_t rev[20];          /* up to 20 digits for 64‑bit n */
    unsigned digits = 0;

    /* extract digits from least significant to most significant */
    while (tmp > 0) {
        rev[digits++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t res = 0;

    /* process digits from most significant to least,
       inserting the product between adjacent digits          */
    for (int i = (int)digits - 1; i >= 0; --i) {
        uint8_t cur = rev[i];
        res = res * 10 + (seq_t)cur;          /* append current digit */

        if (i > 0) {                          /* there is a next digit */
            uint8_t prod = (cur * rev[i - 1]) % 10;
            res = res * 10 + (seq_t)prod;      /* insert product modulo 10 */
        }
    }

    return res;
}
