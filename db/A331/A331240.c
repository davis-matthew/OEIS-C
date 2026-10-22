/*
 * A331240  a(n) = number of triangles with integer sides i <= j <= k with
 *          diameter of circumcircle <= n.
 * https://oeis.org/A331240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of integer triangles i ≤ j ≤ k, i + j > k, with
 *            circumdiameter ≤ n. The condition 2R ≤ n with R = (i·j·k)/(4·Δ)
 *            (Δ = area of the triangle) squared gives the integer inequality
 *            4 i² j² k² ≤ n² (a + b + c)(‑a + b + c)(a ‑ b + c)(a + b ‑ c)
 *            For fixed i = a, j = b the inequality holds f...
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* integer square root of a 128‑bit unsigned integer */
static inline unsigned long long isqrt128(unsigned __int128 x)
{
    long double d = sqrtl((long double)x);
    unsigned long long r = (unsigned long long)d;
    unsigned __int128 rr = r;
    while (rr * rr > x) --rr;
    while ((rr + 1) * (rr + 1) <= x) ++rr;
    return (unsigned long long)rr;
}
static inline unsigned long long ceil_sqrt128(unsigned __int128 x)
{
    unsigned long long r = isqrt128(x);
    unsigned __int128 sq = (unsigned __int128)r * r;
    return sq == x ? r : r + 1;
}

/* the requested sequence */
seq_t a(idx_t n)
{
    seq_t count = 0;
    const unsigned __int128 P = (unsigned __int128)n * n;   /* P = n² */

    for (idx_t a = 1; a <= n; ++a)
    {
        const unsigned __int128 a128 = (unsigned __int128)a;
        const unsigned __int128 a2   = a128 * a128;

        for (idx_t b = a; b <= n; ++b)
        {
            const unsigned __int128 b128 = (unsigned __int128)b;
            const unsigned __int128 b2   = b128 * b128;

            /* S = a + b, d = a – b */
            const unsigned __int128 S = a128 + b128;            /* S <= 2n */
            const unsigned __int128 S2 = S * S;                /* S² */
            const int64_t d_signed = (int64_t)a - (int64_t)b;   /* can be negative */
            const unsigned __int128 d2 = (unsigned __int128)((int64_t)d_signed * (int64_t)d_signed);

            /* coefficients of the quadratic:  P X² + Q X + R = 0  (upwards) */
            const unsigned __int128 qa = 4 * a2 * b2;                        /* 4a²b²  ≤ 4·10¹⁶ */
            const unsigned __int128 qb = P * (S2 + d2);                      /* n²(S² + d²) */
            const signed __int128 Q = (signed __int128)qa - (signed __int128)qb;/* signed  */
            const unsigned __int128 R = P * S2 * d2;

            /* discriminant D = Q² – 4PR  (non‑negative for possible solutions) */
            const signed __int128 D128 = (signed __int128)Q * (signed __int128)Q
                                           - (signed __int128)4 * (signed __int128)P
                                               * (signed __int128)R;
            if (D128 < 0) continue;                /* no real roots → no solutions */

            const unsigned __int128 D  = (unsigned __int128)D128;
            const unsigned __int128 sqrtD = isqrt128(D);             /* floor √D */

            const unsigned __int128 den = 2 * P;                     /* divisor 2P */

            /* numerator of the two roots:  (-Q ± √D) */
            const signed __int128 low_num = -(signed __int128)Q - (signed __int128)sqrtD;
            const signed __int128 high_num = -(signed __int128)Q + (signed __int128)sqrtD;

            if (high_num <= 0) continue;            /* both roots ≤ 0 → no positive X */

            /* lower root X_low: ceil( low_num / den ) if low_num>0, otherwise 0   */
            unsigned __int128 X_low;
            if (low_num <= 0) X_low = 0;
            else {
                X_low = (unsigned __int128)( (low_num + (signed __int128)den - 1) / (signed __int128)den );
            }

            /* upper root X_high : floor( high_num / den ) */
            unsigned __int128 X_high = (unsigned __int128)( high_num / (signed __int128)den );

            /* triangle bounds in terms of X = k²  */
            const unsigned __int128 B1 = (unsigned __int128)b * b;                       /* b² */
            const unsigned __int128 B2 = (unsigned __int128)(a + b - 1) * (a + b - 1);    /* (a+b-1)² */
            const unsigned __int128 B3 = (unsigned __int128)n * n;                       /* n² */

            if (X_high < B1) continue;   /* all admissible X are too small */

            unsigned __int128 lowX = X_low > B1 ? X_low : B1;
            unsigned __int128 highX = X_high;

            if (highX > B2) highX = B2;
            if (highX > B3) highX = B3;
            if (highX < lowX) continue;

            /* convert X bounds to k bounds */
            unsigned long long k_low  = ceil_sqrt128(lowX);
            unsigned long long k_high = isqrt128(highX);

            if (k_low < b)          k_low = b;
            if (k_high > (unsigned long long)(a + b - 1)) k_high = (unsigned long long)(a + b - 1);
            if (k_high > (unsigned long long)n)             k_high = (unsigned long long)n;

            if (k_low <= k_high)
                count += (seq_t)(k_high - k_low + 1);
        }
    }
    return count;
}
