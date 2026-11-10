/*
 * A380562  Rectangular array R read by descending antidiagonals: (row 1) =
 *          (R(1,k)) = (A006337(k)), k >= 1; (row n+1) = inverse runlength
 *          sequence of row n; and R(n,1) = (1, 2, 2, 1, 2, 2, 1, 2, 2, ...) =
 *          (A130196(n)) for n >= 1. See Comments.
 * https://oeis.org/A380562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   R(1,k)=A006337(k); R(n+1,.) = inverse runlength of R(n,.) with
 *            R(n+1,1)=A130196(n); a(n) read by descending antidiagonals:
 *            a(T(d-1)+k)=R(k,d+1-k)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned __int128 A006337_isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 r = 0, bit = (unsigned __int128)1 << 126;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static unsigned __int128 A006337_floor_m_sqrt2(uint64_t m)
{
    unsigned __int128 mm = (unsigned __int128)m;
    return A006337_isqrt_u128(2 * mm * mm);
}

static int A006337_val(uint64_t n)
{
    unsigned __int128 d = A006337_floor_m_sqrt2(n + 1) - A006337_floor_m_sqrt2(n);
    return (int)d;
}

seq_t a(idx_t n)
{
    uint64_t d = 1;
    while ((d * (d + 1)) / 2 < (uint64_t)n) d++;
    uint64_t k = (uint64_t)n - (d * (d - 1)) / 2; /* 1..d : row */
    uint64_t row = k;
    uint64_t L = d + 1 - k;                       /* column */

    unsigned char *cur = (unsigned char *)malloc((size_t)L + 1);
    unsigned char *nxt = (unsigned char *)malloc((size_t)L + 1);
    uint64_t i;
    for (i = 0; i < L; i++) cur[i] = (unsigned char)A006337_val(i + 1);

    for (uint64_t j = 1; j < row; j++) {
        /* build row j+1: first term is A130196(j) */
        int v = (j % 3 == 0) ? 1 : 2;
        uint64_t pos = 0;
        for (i = 0; i < L && pos < L; i++) {
            for (int t = 0; t < cur[i] && pos < L; t++) nxt[pos++] = (unsigned char)v;
            v = 3 - v;
        }
        unsigned char *tmp = cur; cur = nxt; nxt = tmp;
    }
    int res = cur[L - 1];
    free(cur);
    free(nxt);
    return (seq_t)res;
}
