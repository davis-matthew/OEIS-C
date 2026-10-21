/*
 * A287797  Triangle read by rows: T(n,k) gives the independence number of the
 *          k X n knight graph.
 * https://oeis.org/A287797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the independence number of the k×r knight graph, *
 *            where r is the unique integer with r(r+1)/2 ≥ n and k = n −
 *            r(r−1)/2. * The independence number I(r,k) is: * * • if r==1 or
 *            k==1 → r·k * • if r==2 or k==2 → 4·⌊L/4⌋ + 2·min(2, L mod 4)
 *            with L = max(r,k) * • otherwise (r>2, k>2) → ⌈r·k...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   independence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/independence.h"

static inline idx_t row_index(idx_t n)
{
    /* binary search for the smallest r such that r(r+1)/2 >= n */
    idx_t low = 0;
    idx_t high = 1;
    /* grow high until it satisfies the condition */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for index < 1; return 0 for safety */

    idx_t r = row_index(n);
    unsigned __int128 prev_tmp = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev_tmp;
    idx_t k = n - prev;

    return independence(r, k);
}
