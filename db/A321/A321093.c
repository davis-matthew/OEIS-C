/*
 * A321093  Continued fraction expansion of the constant z that satisfies:
 *          CF(4*z, n) = CF(z, n) + 21, for n >= 0, where CF(z, n) denotes the
 *          n-th partial denominator in the continued fraction expansion of z.
 * https://oeis.org/A321093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 6 for n > 0: if n % 3 == 0: a(n) = 5 if n % 3 == 1: a(n)
 *            = s((n-1)/3) if n % 3 == 2: a(n) = 4 - s((n-2)/3) where s(k) =
 *            a(3*k+1) satisfies: s(0) = 1 k % 3 == 0 : s(k) = 1 k % 3 == 2 :
 *            s(k) = 3 k % 3 == 1 : s(k) = 4 - s((k-1)/3)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   s_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/s_of_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 6;
    switch (n % 3)
    {
        case 0: /* 3*n with n ≥ 1 */
            return 5;
        case 1:
            return s_of_k((n - 1) / 3);
        case 2:
            return 4 - s_of_k((n - 2) / 3);
    }
    /* unreachable */
    return 0;
}
