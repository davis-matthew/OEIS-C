/*
 * A249733  Number of (not necessarily distinct) multiples of 9 on row n of
 *          Pascal's triangle.
 * https://oeis.org/A249733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k∈[0,n] such that the exponent of 3 in C(n,k)
 *            is ≥ 2. By Kummer's theorem this equals the number of k for
 *            which the subtraction n–k, performed in base 3, produces at
 *            least two borrows. A simple DP over the digits of n counts such
 *            k in O(log₃ n) time
 * Range:     u8 n <= 269, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return a(n) */
seq_t a(idx_t n)
{
    /* Special case n = 0 (row 0 has only C(0,0)=1) */
    if (n == 0)
        return (seq_t)0;

    /* Convert n to base 3 digits (least significant first) */
    idx_t tmp = n;
    idx_t digits[64];   /* enough for all 64‑bit numbers */
    size_t L = 0;
    while (tmp > 0) {
        digits[L++] = tmp % 3;
        tmp /= 3;
    }

    /* dp[borrow_in][borrows_done] – borrows_done ∈ {0,1,≥2} */
    seq_t dp[2][3];
    seq_t ndp[2][3];
    for (int bi = 0; bi < 2; ++bi)
        for (int bor = 0; bor < 3; ++bor)
            dp[bi][bor] = (seq_t)0;

    dp[0][0] = (seq_t)1;  /* before any digit, no borrow, no borrows */

    for (size_t pos = 0; pos < L; ++pos) {
        int d = (int)digits[pos];
        /* zero new dp */
        for (int bi = 0; bi < 2; ++bi)
            for (int bor = 0; bor < 3; ++bor)
                ndp[bi][bor] = (seq_t)0;

        for (int bi = 0; bi < 2; ++bi) {          /* current borrow_in */
            for (int bor = 0; bor < 3; ++bor) {   /* borrows done so far */
                seq_t cur = dp[bi][bor];
                if (cur == (seq_t)0) continue;

                for (int k = 0; k <= 2; ++k) {     /* digit of k */
                    int diff = d - k - bi;         /* n_i - k_i - borrow_in */
                    int new_borrow;
                    int new_bor;
                    if (diff >= 0) {
                        new_borrow = 0;
                        new_bor = bor;             /* no new borrow */
                    } else {
                        new_borrow = 1;
                        new_bor = bor + 1;
                        if (new_bor > 2) new_bor = 2; /* cap at ≥2 */
                    }
                    ndp[new_borrow][new_bor] += cur;
                }
            }
        }
        /* copy ndp back to dp for next digit */
        for (int bi = 0; bi < 2; ++bi)
            for (int bor = 0; bor < 3; ++bor)
                dp[bi][bor] = ndp[bi][bor];
    }

    /* We need states with final borrow = 0 and at least two borrows.
       (State 2 represents “≥ 2”). */
    return dp[0][2];
}
