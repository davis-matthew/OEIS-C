/*
 * A287455  Start with 0 and repeatedly substitute 0->012, 1->201, 2->210.
 * https://oeis.org/A287455
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

/* Sequence A287455: a(n) is the nth symbol of the fixed point of the
 * morphism 0 → 012, 1 → 201, 2 → 210, starting with symbol 0.
 * The value is obtained by writing n‑1 in base‑3 and following the
 * substitution tree: starting from 0, for each digit d (most
 * significant first) replace the current symbol s by the d‑th symbol of
 * s’s substitution. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; n==0 is outside the sequence but we return 0
       for safety. */
    if (n == 0) return 0;

    /* digits of (n-1) in base 3 (least significant first) */
    idx_t idx = n - 1;
    unsigned char digits[64];
    int len = 0;
    while (idx > 0) {
        digits[len++] = (unsigned char)(idx % 3);
        idx /= 3;
    }
    if (len == 0) return 0;  /* n == 1 */

    /* Mapping table: new_symbol = sigma[old_symbol][digit] */
    const unsigned char sigma[3][3] = {
        {0, 1, 2},   /* 0 → 012 */
        {2, 0, 1},   /* 1 → 201 */
        {2, 1, 0}    /* 2 → 210 */
    };

    unsigned char sym = 0;   /* start with symbol 0 */
    for (int i = len - 1; i >= 0; --i) {
        sym = sigma[sym][digits[i]];
    }
    return (seq_t)sym;
}
