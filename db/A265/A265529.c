/*
 * A265529  Largest base-3 palindrome m <= n, written in base 10.
 * https://oeis.org/A265529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the largest base‑3 palindrome m such that m ≤ n (both in
 *            decimal). The algorithm builds a base‑3 representation of n,
 *            mirrors the most significant half to form a palindrome, then
 *            subtracts one from the prefix if necessary to keep the value ≤
 *            n. The resulting palindrome is returned
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   pow3k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow3k.h"

seq_t a(idx_t n)
{
    /* handle n = 0 explicitly */
    if (n == 0)
        return 0;

    seq_t limit = n;                  /* cast index to seq_t */
    seq_t tmp   = limit;

    /* base‑3 digits of n (least significant first) */
    seq_t digits[64];                 /* 64 > log_3(2^64) */
    unsigned int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    /* reverse so that digits[0] is most significant */
    for (unsigned int i = 0; i < len / 2; ++i) {
        seq_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    unsigned int half = (len + 1) / 2;

    /* build palindrome from the unchanged prefix */
    seq_t cand = 0;
    for (unsigned int i = 0; i < len; ++i) {
        seq_t d = (i < half) ? digits[i] : digits[len - 1 - i];
        cand = cand * 3 + d;
    }
    if (cand <= limit)
        return cand;                  /* mirror itself is already ≤ n */

    /* decide whether a smaller prefix exists */
    bool has_lower = false;
    if (digits[0] > 1) {
        has_lower = true;
    } else {
        for (unsigned int i = 1; i < half; ++i) {
            if (digits[i] != 0) {
                has_lower = true;
                break;
            }
        }
    }

    /* if no smaller prefix, the best palindrome has one digit less */
    if (!has_lower) {
        seq_t max_short = pow3k(len - 1) - 1;   /* 3^(len‑1) – 1 */
        return max_short;
    }

    /* decrement the prefix by one (in base‑3) */
    int idx = (int)half - 1;
    while (idx >= 0) {
        if (digits[idx] > 0) {
            digits[idx]--;
            break;
        } else {
            digits[idx] = 2;
            --idx;
        }
    }

    /* rebuild palindrome after decrement */
    seq_t cand2 = 0;
    for (unsigned int i = 0; i < len; ++i) {
        seq_t d = (i < half) ? digits[i] : digits[len - 1 - i];
        cand2 = cand2 * 3 + d;
    }

    /* choose the larger of the two candidates */
    seq_t max_short = pow3k(len - 1) - 1;
    return (cand2 > max_short) ? cand2 : max_short;
}
