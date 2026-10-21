/*
 * A299504  Triangle read by rows, T(n,k) = (k+1)^(n-k)*k! for 0 <= k <= n.
 * https://oeis.org/A299504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1)^(r-k) * k! (flattened triangle entry) where r and k
 *            satisfy: 0 <= k <= r n < (r+1)(r+2)/2 and n >= r(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 100, u64 n <= 245,
 *            u128 n <= 651, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r: smallest integer with total entries up to row r > n */
    idx_t r = 0;
    idx_t total = 0;          /* entries before current row r */
    while (1) {
        idx_t rowlen = r + 1; /* number of entries in row r */
        if (total + rowlen > n)
            break;
        total += rowlen;
        r++;
    }
    /* k is the index within row r */
    idx_t k = n - total;      /* 0 <= k <= r */

    /* compute k! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= k; ++i)
        fact *= (seq_t)i;

    /* compute (k+1)^(r-k) */
    seq_t pow = 1;
    seq_t base = (seq_t)(k + 1);
    idx_t exp = r - k;
    for (idx_t i = 0; i < exp; ++i)
        pow *= base;

    return fact * pow;
}
