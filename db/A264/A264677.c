/*
 * A264677  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change -1,-1 1,0 -1,-2 -2,-2 or
 *          0,1.
 * https://oeis.org/A264677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 5*a(n-3) + 7*a(n-4) -5*a(n-5) +
 *            7*a(n-6) - 3*a(n-7) + a(n-8) - a(n-9) Initial values: a(1)=1,
 *            a(2)=2, a(3)=10, a(4)=29, a(5)=75, a(6)=201, a(7)=588,
 *            a(8)=1700, a(9)=4785
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined, return 0 for safety */

    const seq_t init[9] = {1, 2, 10, 29, 75, 201, 588, 1700, 4785};
    static seq_t *memo = NULL;
    static size_t memo_len = 0, memo_cap = 0;
    idx_t idx = n - 1;

    /* Allocate if this is the first call */
    if (!memo) {
        memo_cap = 16;
        memo = seq_alloc(memo_cap);
        for (size_t i = 0; i < 9 && i < memo_cap; ++i)
            memo[i] = init[i];
        memo_len = 9;
    }

    /* Expand capacity if necessary */
    if ((size_t)(idx + 1) > memo_cap) {
        size_t new_cap = memo_cap;
        while (new_cap <= (size_t)(idx + 1))
            new_cap <<= 1;
        memo = seq_realloc(memo, memo_cap, new_cap);
        memo_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    if ((size_t)(idx + 1) > memo_len) {
        for (size_t k = memo_len; k <= (size_t)idx; ++k) {
            seq_t pos = 0, neg = 0;
            if (k >= 1) pos += (seq_t)3 * memo[k - 1];
            if (k >= 2) neg += (seq_t)3 * memo[k - 2];
            if (k >= 3) pos += (seq_t)5 * memo[k - 3];
            if (k >= 4) pos += (seq_t)7 * memo[k - 4];
            if (k >= 5) neg += (seq_t)5 * memo[k - 5];
            if (k >= 6) pos += (seq_t)7 * memo[k - 6];
            if (k >= 7) neg += (seq_t)3 * memo[k - 7];
            if (k >= 8) pos += (seq_t)1 * memo[k - 8];
            if (k >= 9) neg += (seq_t)1 * memo[k - 9];
            memo[k] = pos - neg;
        }
        memo_len = idx + 1;
    }

    return memo[idx];
}
