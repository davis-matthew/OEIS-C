/*
 * A287447  Start with 0 and repeatedly substitute 0->012, 1->201, 2->102.
 * https://oeis.org/A287447
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

/* a(n) = nth symbol of the fixed point of the morphism
 *        0 → 0 1 2
 *        1 → 2 0 1
 *        2 → 1 0 2
 *
 * The algorithm converts n-1 to base 3 and follows the
 * substitution tree.  Complexity O(log₃ n). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)            /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* base‑3 digits of (n‑1), least significant first */
    idx_t idx = n - 1;
    unsigned char digits[64];
    int len = 0;
    while (idx > 0) {
        digits[len++] = (unsigned char)(idx % 3);
        idx /= 3;
    }
    /* if n == 1, len == 0 and digits array remains unused */

    /* substitution table */
    static const unsigned char trans[3][3] = {
        {0, 1, 2},    /* 0 → 0 1 2 */
        {2, 0, 1},    /* 1 → 2 0 1 */
        {1, 0, 2}     /* 2 → 1 0 2 */
    };

    unsigned char sym = 0;       /* start with the root symbol 0 */
    for (int i = len - 1; i >= 0; --i) {
        sym = trans[sym][digits[i]];
    }

    return (seq_t)sym;
}
