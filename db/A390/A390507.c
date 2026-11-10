/*
 * A390507  a(n) = number of 1's in A390505(n).
 * https://oeis.org/A390507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A390505(n)): number of 1's in the AN-run encoding
 *            of the n-th binary word (ordered by length, then
 *            lexicographically). The code counts the 1 bits of each run
 *            length plus the run bit, directly
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t m = n - 1;
    unsigned len = 1;
    while (len < 63 && m >= ((idx_t)1 << len)) {
        m -= ((idx_t)1 << len);
        ++len;
    }
    /* word bits: bit i (0-based from left) = (m >> (len-1-i)) & 1 */
    unsigned cur = (unsigned)((m >> (len - 1)) & 1);
    unsigned runlen = 1;
    seq_t res = 0;
    for (unsigned i = 1; i < len; ++i) {
        unsigned b = (unsigned)((m >> (len - 1 - i)) & 1);
        if (b == cur) {
            ++runlen;
        } else {
            unsigned t = runlen;
            while (t) { res += (t & 1); t >>= 1; }
            res += cur;
            cur = b;
            runlen = 1;
        }
    }
    {
        unsigned t = runlen;
        while (t) { res += (t & 1); t >>= 1; }
        res += cur;
    }
    return res;
}
