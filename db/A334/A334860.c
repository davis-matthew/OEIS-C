/*
 * A334860  a(0) = 1, a(1) = 2, after which, a(2n) = A334747(a(n)), a(2n+1) =
 *          a(n)^2.
 * https://oeis.org/A334860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(1) = 2; a(2n) = A334747(a(n)), a(2n+1) = a(n)^2.
 *            Computed on prime exponent vectors, reading the bits of n from
 *            the top
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t a334860_pr[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n)
{
    uint64_t e[64];
    int i, top;
    seq_t result;
    if (n == 0) return (seq_t)1;
    for (i = 0; i < 64; i++) e[i] = 0;
    e[0] = 1; /* a(1) = 2 */
    top = 63;
    while (!((n >> top) & 1ULL)) top--;
    for (i = top - 1; i >= 0; i--) {
        int j;
        if ((n >> i) & 1ULL) {
            for (j = 0; j < 64; j++) e[j] *= 2;
        } else {
            int k = 0;
            while (k < 63 && (e[k] & 1ULL)) k++;
            for (j = 0; j < k; j++) e[j]--;
            e[k]++;
        }
    }
    result = 1;
    for (i = 0; i < 64; i++) {
        uint64_t ex = e[i];
        seq_t base = (seq_t)a334860_pr[i];
        seq_t pw = 1;
        while (ex) {
            if (ex & 1ULL) pw *= base;
            ex >>= 1;
            if (ex) base *= base;
        }
        result *= pw;
    }
    return result;
}
