/*
 * A382371  Remove all occurrences of a digit from n such that the resulting
 *          number, formed by the remaining digits in their original order, is
 *          as large as possible. If no digits remain, a(n)=0.
 * https://oeis.org/A382371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = maximum over all digits d that appear in n of the integer
 *            obtained by deleting every occurrence of d from the decimal
 *            representation of n. If all digits are removed, a(n) = 0
 * Range:     u8 n <= 1255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case (not required by the spec but harmless). */
    if (n == 0)
        return (seq_t)0;

    /* Store the digits of n in reverse order (least significant first). */
    uint8_t rev[20];          /* 64‑bit n has at most 20 decimal digits. */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Record which digits are present. */
    bool present[10] = { false };
    for (size_t i = 0; i < len; ++i)
        present[rev[i]] = true;

    seq_t best = (seq_t)0;

    /* For each present digit d, rebuild the number after deleting all d's. */
    for (int d = 0; d <= 9; ++d) {
        if (!present[d])
            continue;
        seq_t cur = (seq_t)0;
        for (int i = (int)len - 1; i >= 0; --i) {
            uint8_t dig = rev[i];
            if (dig != d)
                cur = cur * (seq_t)10 + (seq_t)dig;
        }
        if (cur > best)
            best = cur;
    }

    return best;
}
