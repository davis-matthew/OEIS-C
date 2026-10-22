/*
 * A366965  Numbers whose difference between the largest and smallest digits
 *          is equal to 8.
 * https://oeis.org/A366965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth smallest integer whose maximum digit minus the minimum digit equals 8.
 * Computed by combinatorial counting of prefixes.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static seq_t count_len(idx_t L) {
    if (L < 2) return 0;
    seq_t pow9 = pow_uint(9, L - 1);
    seq_t pow8 = pow_uint(8, L - 1);
    seq_t pow7 = pow_uint(7, L);
    seq_t res = pow9 * 17;
    res -= pow8 * 31;
    res += pow7 * 2;
    return res;
}

static seq_t count_cat0(bool valid, bool seen0, bool seen8, idx_t rem) {
    if (!valid) return 0;
    seq_t total = pow_uint(9, rem);
    seq_t c1 = 0, c2 = 0, c12 = 0;
    if (!seen0) c1 = pow_uint(8, rem);
    if (!seen8) c2 = pow_uint(8, rem);
    if (!seen0 && !seen8) c12 = pow_uint(7, rem);
    return total - c1 - c2 + c12;
}

static seq_t count_cat1(bool valid, bool seen1, bool seen9, idx_t rem) {
    if (!valid) return 0;
    seq_t total = pow_uint(9, rem);
    seq_t c1 = 0, c2 = 0, c12 = 0;
    if (!seen1) c1 = pow_uint(8, rem);
    if (!seen9) c2 = pow_uint(8, rem);
    if (!seen1 && !seen9) c12 = pow_uint(7, rem);
    return total - c1 - c2 + c12;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for n < 1, return 0 */

    seq_t target = (seq_t)n;
    seq_t cum = 0;
    idx_t L = 2;                   /* Minimum length with terms */

    /* Find the length L containing the nth term */
    while (true) {
        seq_t cntL = count_len(L);
        if (cum + cntL >= target) break;
        cum += cntL;
        L++;
    }

    seq_t k = target - cum - 1;     /* 0‑based index within length L */

    /* State for each category */
    bool valid0 = true, seen0 = false, seen8 = false;
    bool valid1 = true, seen1 = false, seen9 = false;

    uint8_t digits[L];
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;
        bool chosen = false;
        for (uint8_t d = (pos == 0 ? 1 : 0); d <= 9; ++d) {
            bool nv0 = valid0, ns0 = seen0, ns8 = seen8;
            bool nv1 = valid1, ns1 = seen1, ns9 = seen9;

            /* Update state for category 0–8 */
            if (nv0) {
                if (d > 8) nv0 = false;
                else {
                    if (d == 0) ns0 = true;
                    if (d == 8) ns8 = true;
                }
            }

            /* Update state for category 1–9 */
            if (nv1) {
                if (d < 1) nv1 = false;
                else {
                    if (d == 1) ns1 = true;
                    if (d == 9) ns9 = true;
                }
            }

            seq_t cnt0 = count_cat0(nv0, ns0, ns8, rem);
            seq_t cnt1 = count_cat1(nv1, ns1, ns9, rem);
            seq_t cnt = cnt0 + cnt1;

            if (cnt > k) {
                digits[pos] = d;
                valid0 = nv0; seen0 = ns0; seen8 = ns8;
                valid1 = nv1; seen1 = ns1; seen9 = ns9;
                chosen = true;
                break;
            } else {
                k -= cnt;
            }
        }
        /* Should always find a digit */
    }

    /* Construct the resulting value */
    seq_t res = 0;
    for (idx_t i = 0; i < L; ++i) {
        res = res * 10 + digits[i];
    }
    return res;
}
