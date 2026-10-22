/*
 * A382937  Positive integers that contain an odd digit d immediately preceded
 *          by a digit <= d.
 * https://oeis.org/A382937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the nth integer that contains an odd
   digit d immediately preceded by a digit ≤ d (OEIS A382937) */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a number satisfies the digit condition */
static bool has_property(idx_t x) {
    int digits[20];
    int len = 0;
    while (x > 0) {
        digits[len++] = x % 10;
        x /= 10;
    }
    if (len < 2) return false;          /* need at least two digits */

    int prev = -1;                      /* digit to the left of current */
    for (int i = len - 1; i >= 0; --i) { /* most significant → least */
        int d = digits[i];
        if ((d & 1) && prev >= 0 && prev <= d) {
            return true;
        }
        prev = d;
    }
    return false;
}

/* Main function as required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1; undefined for n==0 */
    idx_t count = 0;
    idx_t num = 10;                     /* start from the first two‑digit number */

    while (1) {
        if (has_property(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
        ++num;
    }
}
