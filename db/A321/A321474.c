/*
 * A321474  Reverse the nonzero digits of n.
 * https://oeis.org/A321474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse non-zero digits of n (base 10)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes OEIS A321474: reverse the nonzero digits of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Temporary array to store digits least‑significant first. */
    idx_t tmp[25] = {0};
    idx_t len = 0;
    idx_t t = n;
    while (t > 0) {
        tmp[len++] = t % 10;
        t /= 10;
    }

    /* Convert to most‑significant first order. */
    uint8_t digits[25];
    for (idx_t i = 0; i < len; ++i) {
        digits[i] = (uint8_t)tmp[len - 1 - i];
    }

    /* Record positions and values of non‑zero digits. */
    idx_t pos[25];
    uint8_t nz[25];
    idx_t cnt = 0;
    for (idx_t i = 0; i < len; ++i) {
        if (digits[i] != 0) {
            pos[cnt] = i;
            nz[cnt]  = digits[i];
            ++cnt;
        }
    }

    /* Replace each non‑zero digit with the corresponding digit from the
       reversed list of non‑zero digits. */
    for (idx_t i = 0; i < cnt; ++i) {
        digits[pos[i]] = nz[cnt - 1 - i];
    }

    /* Reconstruct the integer from the modified digits. */
    seq_t res = 0;
    for (idx_t i = 0; i < len; ++i) {
        res = res * 10 + digits[i];
    }

    return res;
}
