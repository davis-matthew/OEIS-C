/*
 * A342640  a(n) = A342639(n, n).
 * https://oeis.org/A342640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined in OEIS A342640 as * a(n) = A342639(n, n); the
 *            set of exponents of a(n) consists * of the non‑negative integers
 *            that cannot be expressed as the * sum of two non‑negative
 *            integers that are not in the set of * exponents of n. The
 *            following implementation follows this * definition di...
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* bitlen = number of bits needed to encode n */
    idx_t bitlen = 0;
    for (idx_t t = n; t; t >>= 1) ++bitlen;

    /* B is twice the bit length; all necessary indices lie below
     * this value.  (For n = 2^k‑1 the highest bit of a(n) is 2k‑1.) */
    idx_t B = 2 * bitlen;

    /* compute the width of seq_t to avoid shifting by an illegal
     * amount on small types (e.g. uint8_t). */
    const idx_t seq_width = (idx_t)sizeof(seq_t) * CHAR_BIT;
    idx_t limit = B;
    if (limit > seq_width) limit = seq_width;
    if (limit == 0) return 0;

    /* S[e] == 1 iff e is an exponent present in n.
       As B ≤ 128 for all practical inputs we can use a fixed array. */
    static uint8_t S[128];
    for (idx_t e = 0; e < B; ++e) {
        if (e < bitlen)
            S[e] = (n >> e) & 1;
        else
            S[e] = 0;
    }

    seq_t res = 0;
    for (idx_t e = 0; e < limit; ++e) {
        int representable = 0;          /* 1 if e = a + b with a,b∉S */
        for (idx_t a = 0; a <= e; ++a) {
            if (!S[a] && !S[e - a]) {
                representable = 1;
                break;
            }
        }
        if (!representable)
            res |= ((seq_t)1 << e);
    }
    return res;
}
