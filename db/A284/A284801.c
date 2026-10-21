/*
 * A284801  Write in base k, complement, reverse. Case k = 5.
 * https://oeis.org/A284801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(complement base‑5 representation of n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base k = 5 */
    const idx_t base = 5;
    const uint8_t complement = base - 1;    /* 4 */

    /* store digits of n in base‑5 (least‑significant first) */
    uint8_t digits[64];
    unsigned int len = 0;
    idx_t temp = n;
    do {
        digits[len++] = (uint8_t)(temp % base);
        temp /= base;
    } while (temp);

    /* build the complemented and reversed number */
    seq_t res = 0;
    const seq_t base_seq = (seq_t)base;
    for (unsigned int i = 0; i < len; ++i) {
        seq_t d_comp = (seq_t)(complement - digits[i]);  /* complement digit */
        res = res * base_seq + d_comp;
    }

    return res;
}
