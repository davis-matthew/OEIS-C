/*
 * A367055  Triangle read by rows: T(n, k) = A000120(n) + A000120(k), 0 <= k
 *          <= n.
 * https://oeis.org/A367055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = popcount(r) + popcount(k), where r = row index such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2, and k = n - r*(r+1)/2 (column
 *            index)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* find the row number r for a given flattened index n */
static idx_t find_row(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = 0;
    idx_t high = 1;

    /* grow high until its triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;                  /* double high */

    low = high >> 1;                 /* previous value is a valid lower bound */

    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                  /* last r with r*(r+1)/2 <= n */
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 prefix = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prefix;     /* k is in [0, r] */

    seq_t result = (seq_t)__builtin_popcountll(r)
                 + (seq_t)__builtin_popcountll(k);

    return result;
}
