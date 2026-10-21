/*
 * A291770  A binary encoding of the zeros in ternary representation of n.
 * https://oeis.org/A291770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary number whose 1‐bits correspond to the *
 *            non‑leading zero digits in the ternary representation of n. *
 *            For example: ternary(33)=1020 → binary(a(33))=101 (a(33)=5)
 * Range:     u8 n <= 9342, u16 n <= 9342, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* just in case */
    idx_t tmp = n;
    /* maximum number of ternary digits for a 64‑bit integer is < 50,
       but we use 128 to be super safe for arbitrary‑precision builds */
    uint8_t digits[128];
    idx_t len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }
    seq_t result = 0;
    bool seen_nonzero = false;
    const unsigned bitlen = (unsigned)sizeof(seq_t) * 8;
    for (idx_t i = len; i > 0; --i) {
        uint8_t d = digits[i - 1];          /* most significant digit first */
        if (d != 0) {
            seen_nonzero = true;
        } else if (seen_nonzero) {
            unsigned pos = i - 1;           /* position of this zero digit */
            if (pos < bitlen) {
                result |= ((seq_t)1 << pos);
            }
        }
    }
    return result;
}
