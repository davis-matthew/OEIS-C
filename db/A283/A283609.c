/*
 * A283609  Numbers whose largest decimal digit is 6.
 * https://oeis.org/A283609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequence of numbers whose largest decimal digit is 6 (OEIS A283609). */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t pow_small(seq_t base, int exp) {
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= base;
    return r;
}

/* Count numbers of length len (digits 0‑6, leading zeros allowed after first digit)
   that contain at least one digit 6, given whether a 6 has already appeared. */
static inline seq_t count_with_len(int len, bool has6) {
    if (len == 0) return has6 ? 1 : 0;
    if (has6) return pow_small(7, len);
    return pow_small(7, len) - pow_small(6, len);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, but safeguard */
    seq_t N = (seq_t)n;

    seq_t cumulative = 0;
    seq_t pow7 = 1;                      /* 7^(L-1) */
    seq_t pow6 = 1;                      /* 6^(L-1) */
    int L = 1;

    while (1) {
        seq_t cnt_len = 6 * pow7 - 5 * pow6;   /* numbers of length L */
        if (cumulative + cnt_len >= N) break;
        cumulative += cnt_len;
        ++L;
        pow7 *= 7;
        pow6 *= 6;
    }

    seq_t k = N - cumulative;            /* 1‑based index within length L */
    seq_t result = 0;
    bool has6 = false;

    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 6; ++d) {
            bool next_has6 = has6 || (d == 6);
            int rem = L - pos - 1;
            seq_t cnt = count_with_len(rem, next_has6);
            if (k <= cnt) {
                result = result * 10 + (seq_t)d;
                has6 = next_has6;
                break;
            } else {
                k -= cnt;
            }
        }
    }
    return result;
}
