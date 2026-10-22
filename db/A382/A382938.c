/*
 * A382938  Nonnegative integers such that every odd digit except the leftmost
 *          is immediately preceded by a larger digit.
 * https://oeis.org/A382938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by brute‑force: iterate over nonnegative integers
   and test the digit property defined for OEIS A382938. */
#include <stdint.h>

static inline int is_valid(seq_t x)
{
    if (x < 10) return 1;   /* single digits are always valid */

    uint8_t digits[20];
    int len = 0;
    seq_t tmp = x;

    /* Extract digits in reverse order (LSD first). */
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Check the property from most significant to least. */
    int prev = digits[len - 1];          /* most significant digit */
    for (int k = len - 2; k >= 0; --k) {
        int d = digits[k];
        if ((d & 1) && !(prev > d)) return 0;  /* odd digit not preceded by larger */
        prev = d;
    }
    return 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t cand = 0; ; ++cand) {
        if (is_valid(cand)) {
            ++count;
            if (count == n) return cand;
        }
    }
}
