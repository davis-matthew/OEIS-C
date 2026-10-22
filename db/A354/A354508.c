/*
 * A354508  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d+1) * d^2 )/(k *
 *          (n-k)!).
 * https://oeis.org/A354508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d+1) * d^2 )/(k *
 *            (n-k)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum  = 0;               /* accumulated result              */
    seq_t fall = 1;               /* falling factorial: n!/(n-k)!   */

    for (idx_t k = 1; k <= n; ++k) {
        /* extend the falling factorial by (n-k+1) */
        seq_t mult = (seq_t)(n - k + 1);
        fall = fall * mult;

        /* compute A078306(k) = Σ_{d|k} (-1)^(k/d+1) * d^2  */
        long long yk = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t e = k / d;
                /* sign for divisor d depends on parity of e = k/d  */
                long long sign_d = (e & 1LL) ? 1LL : -1LL;
                yk += sign_d * (long long)d * (long long)d;

                if (d != e) {
                    /* sign for divisor e depends on parity of d */
                    long long sign_e = (d & 1LL) ? 1LL : -1LL;
                    yk += sign_e * (long long)e * (long long)e;
                }
            }
        }

        /* term = A078306(k) * falling / k */
        seq_t fall_div_k = fall / (seq_t)k;
        seq_t term = (seq_t)yk * fall_div_k;
        sum += term;
    }

    return sum;
}
