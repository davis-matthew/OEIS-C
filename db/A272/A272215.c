/*
 * A272215  a(n) = that number formed by permuting the digits of n which is
 *          divisible by the highest power of 2 (in case of a tie, choose the
 *          smallest number).
 * https://oeis.org/A272215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number obtained by permuting the digits of n that is
 *            divisible by the highest power of 2; if tied, the smallest such
 *            number is returned. Leading zeros are not allowed in
 *            permutations
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   permute_dig, trailing_zeros_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/permute_dig.h"
#include "../utils/trailing_zeros_seq.h"

seq_t a(idx_t n)
{
    /* Convert n to decimal digits (most significant first) */
    int digits[20];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to get most significant first */
        for (int i = 0; i < len / 2; i++) {
            int tmpd = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmpd;
        }
    }

    /* Count digit frequencies */
    unsigned short cnt[10] = {0};
    for (int i = 0; i < len; i++) {
        cnt[digits[i]]++;
    }

    seq_t best_val = 0;
    int best_exp = -1;
    permute_dig(0, 0, len, cnt, &best_val, &best_exp);
    return best_val;
}
