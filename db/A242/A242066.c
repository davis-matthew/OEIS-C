/*
 * A242066  The smallest even k such that lpf_3(k-3) > lpf_3(k-1) >= p_n,
 *          where lpf_3(n) = lpf(n/3^t) (cf. A020639) such that 3^t (t>=0) is
 *          the maximal power of 3 which divides n.
 * https://oeis.org/A242066
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even k such that lpf_3(k-3) > lpf_3(k-1) >= p_n,
 *            where lpf_3(n) removes all factors of 3 from n and then returns
 *            the largest prime factor (lpf)
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, lpf, strip_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"
#include "../utils/get_prime.h"
#include "../utils/strip_three.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* undefined below offset */

    idx_t pn = get_prime(n);      /* p_n */
    for (idx_t k = 4; ; k += 2)   /* k must be even */
    {
        idx_t lpf1 = lpf(strip_three(k - 1));   /* lpf_3(k-1) */
        idx_t lpf2 = lpf(strip_three(k - 3));   /* lpf_3(k-3) */
        if (lpf1 >= pn && lpf2 > lpf1)
            return (seq_t)k;
    }
}
