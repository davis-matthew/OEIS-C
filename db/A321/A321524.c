/*
 * A321524  Right-rotate nonzero digits in ternary expansion of n and convert
 *          back to decimal.
 * https://oeis.org/A321524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = right-rotate nonzero digits in ternary expansion of n and
 *            convert back to decimal
 * Range:     u8 n <= 244, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: 0 maps to 0 */
    if (n == 0)
        return 0;

    /* Convert n to ternary digits (least‑significant first). */
    unsigned char digits[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digits[len++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }

    /* Collect positions of non‑zero digits from most‑significant to least‑significant. */
    int nz_pos[64];
    int nz_len = 0;
    for (int i = len - 1; i >= 0; --i)
        if (digits[i] != 0)
            nz_pos[nz_len++] = i;

    /* Right‑rotate the non‑zero digits in place. */
    if (nz_len > 1) {
        unsigned char last = digits[nz_pos[nz_len - 1]];
        for (int j = nz_len - 1; j >= 1; --j)
            digits[nz_pos[j]] = digits[nz_pos[j - 1]];
        digits[nz_pos[0]] = last;
    }

    /* Reconvert the modified ternary digits back to a decimal (seq_t) value. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 3 + digits[i];

    return res;
}
