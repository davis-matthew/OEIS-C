/*
 * A321800  Delete the decimal digits of n that appear exactly once; write -1
 *          if all digits disappear.
 * https://oeis.org/A321800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = delete all decimal digits of n that occur exactly once;
 *            if the resulting string is empty, return -1; otherwise remove
 *            leading zeros (unless the result is 0)
 * Range:     i8 n <= 221, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count digit frequencies */
    unsigned char freq[10] = {0};
    uint64_t tmp = n;
    if (tmp == 0) {
        freq[0] = 1;
    } else {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            freq[d]++;
            tmp /= 10;
        }
    }

    /* Extract digits in reverse order */
    int digits[20];
    int len = 0;
    tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Keep only digits that appear more than once, preserving order */
    int kept[20];
    int kept_len = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (freq[d] > 1) {
            kept[kept_len++] = d;
        }
    }

    if (kept_len == 0) return -1;   /* all digits disappeared */

    /* Skip leading zeros unless the result is 0 */
    int first = 0;
    while (first < kept_len && kept[first] == 0) first++;
    if (first == kept_len) return 0; /* result consists of only zeros */

    /* Build the numeric result */
    seq_t val = 0;
    for (int i = first; i < kept_len; ++i) {
        val = val * 10 + kept[i];
    }
    return val;
}
