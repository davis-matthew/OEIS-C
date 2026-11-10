/*
 * A390506  a(n) = length of A390505(n).
 * https://oeis.org/A390506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of decimal digits of A390505(n) = sum over the
 *            runs of the n-th binary word (ordered by length, then
 *            lexicographically, as in A076478) of (bitlength(run length) + 1)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t len = 1;
    idx_t total = 2;
    while (total < n) {
        ++len;
        total += ((idx_t)1 << len);
    }
    idx_t sumPrev = total - ((idx_t)1 << len);
    idx_t offset = n - sumPrev - 1;

    unsigned cur = (unsigned)((offset >> (len - 1)) & 1);
    unsigned runlen = 1;
    unsigned digits = 0;
    for (idx_t i = 1; i < len; ++i) {
        unsigned b = (unsigned)((offset >> (len - 1 - i)) & 1);
        if (b == cur) {
            ++runlen;
        } else {
            unsigned t = runlen, d = 0;
            while (t) { ++d; t >>= 1; }
            digits += d + 1;
            cur = b;
            runlen = 1;
        }
    }
    {
        unsigned t = runlen, d = 0;
        while (t) { ++d; t >>= 1; }
        digits += d + 1;
    }
    return (seq_t)digits;
}
