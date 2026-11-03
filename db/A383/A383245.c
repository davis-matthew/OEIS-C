/*
 * A383245  Nonnegative integers that contain the digit 0, or an even digit d
 *          immediately followed by a digit >= d.
 * https://oeis.org/A383245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer that contains digit 0 or
 *            has an even digit d immediately followed by a digit >= d
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9668 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_valid_property(seq_t x)
{
    if (x == (seq_t)0) return true;          /* 0 contains digit 0 */
    seq_t temp = x;
    seq_t prev = (seq_t)10;                  /* sentinel: no next digit yet */
    while (temp > (seq_t)0) {
        seq_t cur = temp % (seq_t)10;        /* current least significant digit */
        if (cur == (seq_t)0) return true;    /* digit 0 found */
        if (prev <= (seq_t)9) {
            if ((cur & (seq_t)1) == (seq_t)0 && prev >= cur) return true;
        }
        prev = cur;
        temp = temp / (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = (idx_t)0;
    seq_t current = (seq_t)0;
    while (1) {
        if (has_valid_property(current)) {
            count += (idx_t)1;
            if (count == n) return current;
        }
        current += (seq_t)1;
    }
}
