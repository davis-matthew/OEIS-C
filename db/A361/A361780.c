/*
 * A361780  Numbers that have digits consisting only of line segments {1, 4,
 *          7} or curved digits {0, 3, 6, 8, 9}.
 * https://oeis.org/A361780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = number obtained by taking (n-1) in base‑8, mapping * each
 *            base‑8 digit d to 0→0,1→1,2→3,3→4,4→6,5→7,6→8,7→9, and
 *            interpreting * the resulting decimal digits as a base‑10 number
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t map[8] = {0, 1, 3, 4, 6, 7, 8, 9};

    idx_t m = n - 1ULL;          /* 0‑based index */
    idx_t pw = 1ULL;             /* highest power of 8 <= m */
    while (pw <= m / 8ULL) pw *= 8ULL;

    seq_t result = 0;
    while (pw > 0) {
        uint8_t d = (uint8_t)(m / pw);  /* current base‑8 digit */
        m %= pw;
        pw /= 8ULL;
        result = result * 10 + (seq_t)map[d];
    }
    return result;
}
