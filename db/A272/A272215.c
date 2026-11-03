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
 * Validated: standard, pass, 99 terms through n = 9790 (2026-11-01)
 * Helpers:   ctz_seq_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ctz_seq_to_i32.h"

static void permute_dig(int pos, seq_t cur, int len,
                        unsigned short *cnt,
                        seq_t *best_val, int *best_exp)
{
    if (pos == len) {
        int exp = ctz_seq_to_i32(cur);
        if (exp > *best_exp || (exp == *best_exp && cur < *best_val)) {
            *best_exp = exp;
            *best_val = cur;
        }
        return;
    }
    for (int d = 0; d < 10; d++) {
        if (cnt[d] == 0) continue;
        if (pos == 0 && d == 0 && len > 1) continue; /* no leading zero */
        cnt[d]--;
        permute_dig(pos + 1, cur * 10 + d, len, cnt, best_val, best_exp);
        cnt[d]++; /* backtrack */
    }
}

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
