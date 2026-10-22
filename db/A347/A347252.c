/*
 * A347252  Number of nonnegative triples (a, b, c) satisfying (a + b + c <=
 *          n) and (a * b * c <= n).
 * https://oeis.org/A347252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{a=0..n} Sum_{b=0..n-a} Sum_{c=0..n-a-b} [a*b*c <= n]
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t a = 0; a <= n; ++a) {
        for (idx_t b = 0; b <= n - a; ++b) {
            idx_t sum_ab = a + b;
            idx_t cmax1 = n - sum_ab;   /* maximum c from a+b+c <= n */

            if (a == 0 || b == 0) {
                /* product constraint always satisfied when a==0 or b==0 */
                result += (seq_t)cmax1 + (seq_t)1;
            } else {
                idx_t cmax2;
                /* avoid overflow in a*b */
                if (a > n / b) {
                    /* a*b > n  =>  floor(n/(a*b)) == 0 */
                    cmax2 = 0;
                } else {
                    idx_t prod = a * b;
                    cmax2 = n / prod;
                }
                idx_t cmax = (cmax1 < cmax2) ? cmax1 : cmax2;
                result += (seq_t)cmax + (seq_t)1;
            }
        }
    }
    return result;
}
