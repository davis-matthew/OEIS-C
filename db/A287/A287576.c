/*
 * A287576  Start with 0 and repeatedly substitute 0->0321, 1->3210, 2->2103,
 *          3->1032.
 * https://oeis.org/A287576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9589, u16 n <= 9589, u32 n <= 9589, u64 n <= 9589,
 *            u128 n <= 9589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A287576:
   a(n) is the nth symbol (1‑indexed) of the infinite word obtained by
   repeatedly applying the morphism
   0 → 0321, 1 → 3210, 2 → 2103, 3 → 1032 to the initial symbol 0. */

#include <stdint.h>

static const uint8_t morph[4][4] = {
    {0, 3, 2, 1},   /* 0 → 0321 */
    {3, 2, 1, 0},   /* 1 → 3210 */
    {2, 1, 0, 3},   /* 2 → 2103 */
    {1, 0, 3, 2}    /* 3 → 1032 */
};

seq_t a(idx_t n)
{
    /* offset = 1; a(1) is the first symbol of the fixed point. */
    if (n == 0)
        return (seq_t)0;            /* undefined for n < 1 */

    idx_t k = n - 1;                /* zero‑based position in the word */

    /* collect base‑4 digits of k, least significant first */
    uint8_t digits[32];
    int len = 0;
    while (k > 0) {
        digits[len++] = (uint8_t)(k & 3); /* k % 4 */
        k >>= 2;                          /* k /= 4 */
    }

    uint8_t cur = 0;                       /* start from the initial symbol 0 */
    for (int i = len - 1; i >= 0; --i)
        cur = morph[cur][digits[i]];

    return (seq_t)cur;
}
