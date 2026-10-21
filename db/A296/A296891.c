/*
 * A296891  Numbers whose base-13 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth positive integer whose base‑13 digits contain
 * equal numbers of pits and peaks.  A pit is a digit that is smaller than
 * both adjacent digits, a peak is larger than both adjacent digits.
 *
 * The function performs a brute‑force search.  For n ≥ 1 it returns the
 * correct value; for n = 0 it returns 0 (undefined for the offset).
 *
 * The implementation is limited to the maximal representable value of
 * seq_t – if the nth good number does not fit in seq_t the behaviour
 * is unspecified, which is acceptable for the supported builds.
 */
#include <stdbool.h>
#include <stddef.h>

static inline bool is_good(seq_t x)
{
    /* Collect base‑13 digits (least significant first). */
    uint8_t digits[128];
    unsigned int len = 0;

    seq_t tmp = x;
    if (tmp == 0) {
        /* Zero has no pits or peaks, so it satisfies the condition. */
        return true;
    }
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 13);
        tmp /= 13;
    }

    /* Numbers with fewer than three digits have 0 pits and 0 peaks. */
    if (len <= 2)
        return true;

    int pits = 0, peaks = 0;
    for (unsigned int i = 1; i + 1 < len; ++i) {
        uint8_t prev = digits[i - 1];
        uint8_t cur  = digits[i];
        uint8_t next = digits[i + 1];
        if (prev > cur && cur < next) {
            ++pits;
        } else if (prev < cur && cur > next) {
            ++peaks;
        }
    }
    return pits == peaks;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* Undefined for the offset 1. */
        return 0;

    idx_t target = n;    /* The desired rank. */
    idx_t count  = 0;    /* How many good numbers we have seen. */
    seq_t val    = 1;    /* Current candidate. */

    for (;;) {
        if (is_good(val)) {
            ++count;
            if (count == target)
                return val;
        }
        ++val;
    }
}
