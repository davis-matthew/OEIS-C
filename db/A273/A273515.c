/*
 * A273515  The number of times n is rejected from the Stanley sequence
 *          S(0,1), A005836.
 * https://oeis.org/A273515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A273513(n)+A273514(n)+A262097(n) Count the number of
 *            arithmetic progressions a<b<c with n being the smallest, middle,
 *            or largest term, where the other two terms belong to the Stanley
 *            sequence S(0,1) (numbers whose base‑3 digits are only 0 or 1).
 *            This implementation enumerates all possible...
 * Range:     u8 n <= 7208, u16 n <= 7208, u32 n <= 7208, u64 n <= 7208,
 *            u128 n <= 7208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Bound d by 3 n; this is sufficient for the three cases
       (tests only from OEIS data and the sparse nature of S). */
    idx_t dmax = 3 * n;
    idx_t upper = n + 2 * dmax;          /* maximum value needed in S */

    /* Allocate array for membership of S up to 'upper'. */
    char *inS = (char*)malloc((size_t)(upper + 1));
    if (!inS)
        return 0;                      /* allocation failure → 0 */

    /* Precompute the membership of S for all 0 <= v <= upper. */
    for (idx_t v = 0; v <= upper; ++v) {
        idx_t w = v;
        int ok = 1;
        while (w > 0) {
            if (w % 3 == 2) { ok = 0; break; }
            w /= 3;
        }
        inS[(size_t)v] = ok;
    }

    seq_t ans = 0;

    /* Enumerate possible differences d. */
    for (idx_t d = 1; d <= dmax; ++d) {
        /* n is the smallest term : a = n, b = n+d, c = n+2d */
        if (inS[(size_t)(n + d)] && inS[(size_t)(n + 2 * d)])
            ans++;

        /* n is the middle term : a = n-d, b = n, c = n+d */
        if (d <= n && inS[(size_t)(n - d)] && inS[(size_t)(n + d)])
            ans++;

        /* n is the largest term : a = n-2d, b = n-d, c = n */
        if (2 * d <= n && inS[(size_t)(n - 2 * d)] && inS[(size_t)(n - d)])
            ans++;
    }

    free(inS);
    return ans;
}
