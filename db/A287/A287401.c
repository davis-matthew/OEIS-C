/*
 * A287401  Start with 0 and repeatedly substitute 0->012, 1->210, 2->120.
 * https://oeis.org/A287401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the digit obtained by processing the base‑3
 *            representation of n‑1 through the morphism 0→012, 1→210, 2→120
 *            (starting symbol 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int trans[3][3] = {
    {0, 1, 2},   /* from symbol 0 */
    {2, 1, 0},   /* from symbol 1 */
    {1, 2, 0}    /* from symbol 2 */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not used (offset 1) */

    idx_t x = n - 1;                      /* zero‑based position */
    unsigned char digit[64];              /* enough for 64‑bit indices */
    int len = 0;

    /* decompose x into base‑3 digits (least significant first) */
    do {
        digit[len++] = (unsigned char)(x % 3);
        x /= 3;
    } while (x);

    seq_t s = (seq_t)0;                   /* start symbol 0 */

    /* process digits from most significant to least */
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digit[i];
        int si = (int)s;                  /* s is 0..2, safe to cast */
        s = (seq_t)trans[si][d];
    }
    return s;
}
