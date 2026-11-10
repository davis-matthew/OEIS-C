/*
 * A284945  Positions of 0 in A284944; complement of A284946.
 * https://oeis.org/A284945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284944, the fixed point of
 *            0->01, 1->1110. Computed by descending through the morphism
 *            using zero counts and lengths
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9241 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t A284945_pos(idx_t n)
{
    enum { MD = 128 };
    seq_t L[2][MD];
    seq_t Z[2][MD];
    L[0][0] = 1; L[1][0] = 1;
    Z[0][0] = 1; Z[1][0] = 0;
    int d = 0;
    while (Z[0][d] < (seq_t)n && d < MD - 1) {
        ++d;
        L[0][d] = L[0][d-1] + L[1][d-1];
        L[1][d] = 3 * L[1][d-1] + L[0][d-1];
        Z[0][d] = Z[0][d-1] + Z[1][d-1];
        Z[1][d] = 3 * Z[1][d-1] + Z[0][d-1];
    }
    seq_t r = (seq_t)n;
    seq_t offset = 0;
    int letter = 0;
    while (d > 0) {
        int kids[4];
        int cnt;
        if (letter == 0) {
            kids[0] = 0; kids[1] = 1; cnt = 2;
        } else {
            kids[0] = 1; kids[1] = 1; kids[2] = 1; kids[3] = 0; cnt = 4;
        }
        int chosen = kids[cnt - 1];
        for (int i = 0; i < cnt; i++) {
            int c = kids[i];
            seq_t z = Z[c][d-1];
            if (r <= z) {
                chosen = c;
                break;
            }
            r -= z;
            offset += L[c][d-1];
        }
        letter = chosen;
        --d;
    }
    return offset + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    return A284945_pos(n);
}
