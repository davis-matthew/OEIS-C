/*
 * A363741  Number of factorizations of n satisfying (mean) = (median) =
 *          (mode), assuming there is a unique mode.
 * https://oeis.org/A363741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A363741 – Count of multiplicative factorizations of n such that
 *            mean = median = unique mode
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void dfs_factor(idx_t remaining, idx_t start, idx_t *arr,
                       idx_t len, seq_t *cnt)
{
    if (remaining == (idx_t)1) {
        /* evaluate the current factorization stored in arr[0..len-1] */
        seq_t sum = 0;
        for (idx_t i = 0; i < len; ++i)
            sum += (seq_t)arr[i];

        /* find the mode and verify it is unique */
        idx_t maxcnt = 0;
        seq_t mode = 0;
        bool unique = true;
        idx_t curcnt = 1;
        for (idx_t i = 1; i < len; ++i) {
            if (arr[i] == arr[i-1]) {
                ++curcnt;
            } else {
                if (curcnt > maxcnt) {
                    maxcnt = curcnt;
                    mode   = (seq_t)arr[i-1];
                    unique = true;
                } else if (curcnt == maxcnt) {
                    unique = false;
                }
                curcnt = 1;
            }
        }
        /* last run */
        if (curcnt > maxcnt) {
            maxcnt = curcnt;
            mode   = (seq_t)arr[len-1];
            unique = true;
        } else if (curcnt == maxcnt) {
            unique = false;
        }

        if (!unique)
            return;

        /* mean == mode */
        if (sum != mode * (seq_t)len)
            return;

        /* median == mode */
        if (len & 1) {                     /* odd length */
            if ((seq_t)arr[len/2] != mode)
                return;
        } else {                           /* even length */
            if (((seq_t)arr[len/2 - 1] + (seq_t)arr[len/2]) != 2 * mode)
                return;
        }

        ++(*cnt);
        return;
    }

    /* try all divisors f >= start (ensuring non‑decreasing order) */
    for (idx_t f = start; f <= remaining; ++f) {
        if (f < (idx_t)2)
            continue;
        if (remaining % f != 0)
            continue;
        arr[len] = f;
        dfs_factor(remaining / f, f, arr, len + 1, cnt);
    }
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < (idx_t)2)
        return res;            /* a(1) = 0 */
    idx_t stack[64];
    dfs_factor(n, (idx_t)2, stack, (idx_t)0, &res);
    return res;
}
