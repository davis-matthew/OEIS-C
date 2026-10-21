/*
 * A284382  Numbers k with digits 5 and 9 only.
 * https://oeis.org/A284382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest positive integer whose decimal
 *            representation uses only the digits 5 and 9
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for offset < 1 */

    /* Find the digit length of a(n) */
    idx_t len = 0;
    idx_t total = 0;
    while (1) {
        ++len;
        total += ((idx_t)1 << len);   /* add 2^len numbers of this length */
        if (n <= total) break;
    }

    /* Numbers having fewer digits occupy 'prev_total' positions */
    idx_t prev_total = total - ((idx_t)1 << len);
    idx_t offset = n - prev_total - 1;   /* zero‑based offset within the current length block */

    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        seq_t digit = ((offset >> (len - 1 - i)) & 1) ? 9 : 5;
        result = result * 10 + digit;
    }
    return result;
}
