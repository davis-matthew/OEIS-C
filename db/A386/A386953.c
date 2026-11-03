/*
 * A386953  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 9.
 * https://oeis.org/A386953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9226 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{r=0..n}  #{k : 0 ≤ k ≤ r  and  C(r,k) not divisible by 9}
 * 
 * The implementation uses Kummer’s theorem: the exponent of 3 in C(r,k) equals
 * the number of borrows when subtracting k from r in base 3.  We count all
 * k for which at most one borrow occurs, which guarantees the exponent
 * is 0 or 1, i.e. C(r,k) is not divisible by 9.  The algorithm is O(n·log₃n)
 * and works for any n up to the limits described in the problem statement.
 */

#include <stdint.h>

static seq_t count_not_div_by_9(idx_t r)
{
    /* Convert r to base‑3 digits (least significant first). */
    idx_t digits[20];           /* 3¹⁰ = 59049 > 10 000, so 20 is safe. */
    idx_t len = 0;
    idx_t tmp = r;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 3;
            tmp /= 3;
        }
    }

    /* dp[borrow_in][borrow_cnt] where borrow_cnt ∈ {0,1}. */
    seq_t dp[2][2] = {{0, 0}, {0, 0}};
    dp[0][0] = 1;  /* start with no borrow, zero borrowed positions. */

    for (idx_t pos = 0; pos < len; ++pos) {
        seq_t ndp[2][2] = {{0, 0}, {0, 0}};
        for (int bi = 0; bi < 2; ++bi) {            /* borrow_in */
            for (int bc = 0; bc < 2; ++bc) {        /* cumulative borrow_cnt */
                seq_t val = dp[bi][bc];
                if (val == 0) continue;

                int t = (int)digits[pos] - bi;     /* r_i - borrow_in */

                seq_t no_borrow = 0, borrow = 0;
                if (t >= 0)
                    no_borrow = (seq_t)(t + 1);    /* k_i ≤ t */
                else
                    no_borrow = 0;                 /* none without borrow */
                borrow = 3 - no_borrow;           /* remaining k_i values */

                /* Case: no borrow at this digit. */
                if (no_borrow) {
                    ndp[0][bc] += val * no_borrow;
                }
                /* Case: borrow at this digit (adds 1 to borrow count). */
                if (borrow && bc + 1 <= 1) {
                    ndp[1][bc + 1] += val * borrow;
                }
            }
        }
        dp[0][0] = ndp[0][0];
        dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0];
        dp[1][1] = ndp[1][1];
    }
    /* Valid k’s finish with no remaining borrow (borrow_in==0). */
    return dp[0][0] + dp[0][1];
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t r = 0; r <= n; ++r) {
        total += count_not_div_by_9(r);
    }
    return total;
}
