/*
 * A387909  Consider the factorial base expansion of n, without leading zeros,
 *          say (d(1), ..., d(w)); for k = 2..floor(w/2), if d(k) <= k then
 *          exchange d(k) and d(w+1-k); the resulting digits correspond to the
 *          factorial base expansion of a(n).
 * https://oeis.org/A387909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by converting n into its factorial base digits
 *            (d(1),…,d(w)), swapping positions k and w+1−k for 2 ≤ k ≤ ⌊w/2⌋
 *            whenever d(k) ≤ k, and interpreting the resulting digits back as
 *            a factorial‑base number
 * Range:     u8 n <= 241, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned char a_digits[64];    /* digits a_k  (k = 1 … w)  */
    int w = 0;
    int base = 2;                  /* modulus base = k+1 */

    /* compute factorial base digits a_k (least significant first) */
    while (1) {
        a_digits[w++] = (unsigned char)(m % (idx_t)base);
        m /= (idx_t)base;
        if (m == 0)
            break;
        ++base;
    }

    /* reverse to obtain d(1)…d(w) (most significant first) */
    unsigned char d[64];
    for (int i = 0; i < w; ++i)
        d[i] = a_digits[w - 1 - i];

    /* conditional swapping of symmetric digits */
    for (int k = 2; k <= w / 2; ++k) {
        if (d[k - 1] <= (unsigned char)k) {
            unsigned char tmp = d[k - 1];
            d[k - 1] = d[w - k];
            d[w - k] = tmp;
        }
    }

    /* convert back to integer */
    seq_t res = 0;
    for (int k = w; k >= 1; --k) {
        seq_t digit = (seq_t)d[w - k];
        res = res * (seq_t)(k + 1) + digit;
    }

    return res;
}
