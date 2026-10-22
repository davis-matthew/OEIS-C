/*
 * A394798  For n >= 2, the number of bases b <= n for which the average of
 *          the digits of n base b equals the average of the digits of n^2
 *          base b.
 * https://oeis.org/A394798
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases b ≤ n such that
 *            sumDigits(n,b)/digits(n,b) == sumDigits(n^2,b)/digits(n^2,b)
 * Range:     u8 n <= 15, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t ans = 0;
    seq_t n_val = (seq_t)n;
    seq_t n2    = n_val * n_val;

    for (idx_t base = 2; base <= n; ++base)
    {
        /* digits of n in this base */
        seq_t sum_n = 0;
        idx_t  len_n = 0;
        seq_t tmp = n_val;
        while (tmp > 0)
        {
            sum_n += tmp % (seq_t)base;
            ++len_n;
            tmp   /= (seq_t)base;
        }

        /* digits of n^2 in this base */
        seq_t sum_n2 = 0;
        idx_t  len_n2 = 0;
        tmp = n2;
        while (tmp > 0)
        {
            sum_n2 += tmp % (seq_t)base;
            ++len_n2;
            tmp   /= (seq_t)base;
        }

        /* compare the averages via cross multiplication.
           Use 128‑bit intermediate to avoid overflow on small seq_t.  */
        if ( (unsigned __int128)sum_n  * (unsigned __int128)len_n2 ==
             (unsigned __int128)sum_n2 * (unsigned __int128)len_n )
        {
            ++ans;
        }
    }
    return ans;
}
