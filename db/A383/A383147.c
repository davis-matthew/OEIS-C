/*
 * A383147  Sum of odd divisors m of n such that there is a divisor d of n
 *          with d < m < 2*d.
 * https://oeis.org/A383147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors m of n for which there exists a
 *            divisor d of n with d < m < 2*d
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Collect all divisors of n.  The number of divisors is < 2048 for all 64‑bit n. */
    idx_t divs[2048];
    size_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            idx_t j = n / i;
            if (j != i)
                divs[cnt++] = j;
        }
    }

    /* For each odd divisor m, check if a divisor d exists with d < m < 2*d. */
    for (size_t mi = 0; mi < cnt; ++mi) {
        idx_t m = divs[mi];
        if ((m & 1) == 0)          /* skip even divisors */
            continue;
        /* m = 1 never satisfies d < m < 2*d, so it is omitted automatically. */
        for (size_t di = 0; di < cnt; ++di) {
            idx_t d = divs[di];
            if (d < m && d > m / 2) {   /* equivalent to d < m < 2*d, avoiding overflow */
                sum += (seq_t)m;
                break;                  /* only need one such d */
            }
        }
    }

    return sum;
}
