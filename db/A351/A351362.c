/*
 * A351362  Number of ways the numbers from 1..n do not divide the numbers
 *          from n..2n-1.
 * https://oeis.org/A351362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1) - 1 + A006218(n-1) - A006218(2n-1)
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A006218(m) = sum_{k=1..m} floor(m/k) */
#include <stdint.h>

static seq_t sum_floor(idx_t m)
{
    idx_t i = 1;
    seq_t total = 0;
    while (i <= m) {
        idx_t q = m / i;
        idx_t r = m / q;
        total += (seq_t)q * ((seq_t)(r - i + 1));
        i = r + 1;
    }
    return total;
}

seq_t a(idx_t n)
{
    /* a(n) = n*(n+1) - 1 + sum_floor(n-1) - sum_floor(2n-1) */
    seq_t a = (seq_t)n * ((seq_t)n + 1) - 1;
    a += sum_floor(n - 1);
    a -= sum_floor(2 * n - 1);
    return a;
}
