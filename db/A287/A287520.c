/*
 * A287520  Start with 0 and repeatedly substitute 0->012, 1->102, 2->120.
 * https://oeis.org/A287520
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

/* Sequence A287520:
   a(n) is the n‑th symbol of the fixed point of the morphism
   0 → 0 1 2,   1 → 1 0 2,   2 → 1 2 0.
   The term is obtained by reading the base‑3 representation of n‑1
   (most significant digit first) and following the morphism rules. */

#include <stddef.h>
#include <stdint.h>

/* mapping[s][d] gives the image of symbol s under
   the morphism when the next base‑3 digit is d. */
static const unsigned char mapping[3][3] = {
    {0, 1, 2},   /* 0 → 0 1 2 */
    {1, 0, 2},   /* 1 → 1 0 2 */
    {1, 2, 0}    /* 2 → 1 2 0 */
};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n==0 we return 0
       (behavior below the offset does not matter). */
    if (n == 0)
        return 0;

    idx_t m = n - 1;          /* 0‑based position */
    unsigned char digits[64]; /* enough for all n ≤ 2^64‑1 */
    size_t len = 0;

    /* Decompose m into base‑3 digits (least significant first). */
    do {
        digits[len++] = (unsigned char)(m % 3);
        m /= 3;
    } while (m > 0);

    size_t cur = 0;             /* start from symbol 0 */
    /* Traverse from most significant digit to least. */
    while (len--) {
        cur = mapping[cur][digits[len]];
    }

    return (seq_t)cur;
}
