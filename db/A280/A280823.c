/*
 * A280823  Numbers with an odd number of digits and with an odd number of
 *          distinct digits.
 * https://oeis.org/A280823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer having an odd number of
 *            decimal digits and an odd number of distinct decimal digits. The
 *            sequence starts with a(1)=0, a(2)=1,
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 909 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper: test whether x has odd digit length and odd number of distinct digits */
static bool odd_len_and_distinct(seq_t x)
{
    seq_t tmp = x;
    int len = 0;
    unsigned char seen[10] = {0};

    /* Special case for x = 0 */
    if (tmp == 0) {
        /* len = 1 (odd), distinct = 1 (odd) */
        return true;
    }

    while (tmp > 0) {
        unsigned char d = (unsigned char)(tmp % 10);
        seen[d] = 1;
        ++len;
        tmp /= 10;
    }

    int distinct = 0;
    for (int i = 0; i < 10; ++i) {
        if (seen[i]) {
            ++distinct;
        }
    }

    return (len & 1) && (distinct & 1);
}

seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t count = 0;

    while (1) {
        if (odd_len_and_distinct(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
        ++x;
    }
}
