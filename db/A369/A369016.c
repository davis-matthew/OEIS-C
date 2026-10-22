/*
 * A369016  Triangle read by rows: T(n, k) = binomial(n - 1, k - 1) * (k -
 *          1)^(k - 1) * (n - k) * (n - k + 1)^(n - k - 1).
 * https://oeis.org/A369016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) flattened row-wise where T(n, k) = binomial(n-1,
 *            k-1) * (k-1)^(k-1) * (n-k) * (n-k+1)^(n-k-1)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Integer square root of x: floor(sqrt(x)) */
static idx_t int_sqrt(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t m = 8 * n + 1;
    idx_t t = int_sqrt(m);
    idx_t r = (t - 1) / 2;
    idx_t cumPrev = r * (r + 1) / 2;
    idx_t c = n - cumPrev;          /* column index */
    if (c == 0 || c == r) return 0; /* ends of the row are always zero */

    seq_t bin = binomial(r - 1, c - 1);
    seq_t pow1 = big_pow((seq_t)(c - 1), (seq_t)(c - 1));
    seq_t lhs = bin * pow1;
    seq_t mult2 = (seq_t)(r - c);
    idx_t exp2 = r - c - 1;
    seq_t pow2 = big_pow((seq_t)(r - c + 1), (seq_t)exp2);

    return lhs * mult2 * pow2;
}
