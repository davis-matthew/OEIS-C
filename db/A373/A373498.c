/*
 * A373498  a(a(a(n))) = A370655(n).
 * https://oeis.org/A373498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (L-1)*(2L-1) + P where L = ceil((sqrt(8*n+1)-1)/4) R = n
 *            - (L-1)*(2L-1) If R ≤ 2L-1: P = R+1 if R is odd P = 2L+R if R is
 *            even else P = R if R is odd P = 4L-R-1 if R is even
 * Range:     u8 n <= 254, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer sqrt for up to 128‑bit operands (returns floor(sqrt(x))) */
static inline idx_t isqrt128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;           /* maximum 64‑bit value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* compute L = ceil((sqrt(8*n+1)-1)/4) */
    unsigned __int128 target = (unsigned __int128)8 * n + 1;
    idx_t root = isqrt128(target);     /* floor sqrt(target) */
    idx_t L = (root + 1) / 4;          /* starting guess */
    /* adjust to satisfy (4*L+1)^2 >= target */
    while ((unsigned __int128)(4 * L + 1) * (unsigned __int128)(4 * L + 1) < target)
        ++L;

    /* base = (L-1)*(2L-1) */
    seq_t base = (seq_t)(L - 1) * (seq_t)(2 * L - 1);

    /* R = n - base */
    seq_t R = (seq_t)n - base;

    seq_t limit = (seq_t)(2 * L - 1);
    seq_t P;
    if (R <= limit) {
        if ((R & 1) == 0)             /* R even */
            P = (seq_t)2 * (seq_t)L + R;
        else                          /* R odd */
            P = R + 1;
    } else {
        if ((R & 1) == 0)             /* R even */
            P = (seq_t)4 * (seq_t)L - R - 1;
        else                          /* R odd */
            P = R;
    }

    return base + P;
}
