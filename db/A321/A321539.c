/*
 * A321539  3^n with digits rearranged into nonincreasing order.
 * https://oeis.org/A321539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004186(A000244(n)) – digits of 3^n sorted
 *            non‑increasingly
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   digit_counts, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3 = seq_pow((seq_t)3, n);

    /* count decimal digits */
    unsigned char cnt[10];
    digit_counts(pow3, cnt);

    /* build number with digits sorted descending */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned char k = 0; k < cnt[d]; ++k) {
            result = result * 10 + d;
        }
    }
    return result;
}
