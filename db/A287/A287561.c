/*
 * A287561  Start with 0 and repeatedly substitute 0->0213, 1->2130, 2->1302,
 *          3->3021.
 * https://oeis.org/A287561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8930, u16 n <= 8930, u32 n <= 8930, u64 n <= 8930,
 *            u128 n <= 8930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*  Fixed point of the morphism
    0 -> 0213
    1 -> 2130
    2 -> 1302
    3 -> 3021
    a(1) = 0, a(2) = 2, …
    The n‑th term (1‑based) is returned. */

seq_t a(idx_t n)
{
    /* Table for applying the morphism once.  The entries are
       0,1,2,3 – small enough to fit in an unsigned char for
       indexing.  Using seq_t for the table contents is harmless
       because the values themselves are tiny. */
    static const seq_t MAP[4][4] = {
        {0, 2, 1, 3},   /* 0 -> 0213 */
        {2, 1, 3, 0},   /* 1 -> 2130 */
        {1, 3, 0, 2},   /* 2 -> 1302 */
        {3, 0, 2, 1}    /* 3 -> 3021 */
    };

    /* n is 1‑based; the sequence starts with 0 at position 1. */
    if (n == 0)          /* out of bounds – the problem statement
                            allows any behaviour below the offset */
        return 0;

    /* Convert (n‑1) to base‑4 digits (least significant first). */
    idx_t k = n - 1;
    unsigned char digits[64];   /* 64 digits are far more than enough
                                   for a 64‑bit number in base 4. */
    size_t len = 0;
    while (k) {
        digits[len++] = (unsigned char)(k & 3);  /* k % 4 */
        k >>= 2;                                 /* k /= 4 */
    }

    /* Starting from the initial digit 0, apply the morphism once
       for each base‑4 digit, going from the most significant to
       the least significant. */
    unsigned char d = 0;      /* current character, 0..3 */
    while (len--) {
        d = (unsigned char)MAP[d][digits[len]];
    }

    return (seq_t)d;
}
