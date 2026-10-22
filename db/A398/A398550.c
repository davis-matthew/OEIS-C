/*
 * A398550  Largest value occurring in any n-tuple (x_1, ..., x_n) of distinct
 *          positive integers with 1/x_1 + 2/x_2 + ... + n/x_n = n.
 * https://oeis.org/A398550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   For n ≥ 4 a(n) is the largest denominator that occurs in the *
 *            greedy solution of * 1/x1 + 2/x2 + … + n/xn = n * with all xi
 *            distinct. The greedy algorithm is * * 1. Set R = 1/(n+1) (the
 *            remainder after assigning w=n and w=n‑1). * 2. For w = n‑2 … 3
 *            (descending) choose the smallest unused * integ...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;

    /* Allocate array for used denominators: exactly n values will be stored */
    seq_t *used = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    if (!used) return 0;  /* memory failure */

    size_t used_cnt = 0;
    used[used_cnt++] = 1;
    used[used_cnt++] = (seq_t)(n + 1);
    seq_t max_den = (seq_t)(n + 1) > (seq_t)1 ? (seq_t)(n + 1) : (seq_t)1;

    /* Remainder R = num/den */
    seq_t num = 1;
    seq_t den = (seq_t)(n + 1);

    /* Step 2: w = n-2 down to 3 */
    for (idx_t w = n - 2; w > 2; --w) {
        /* x > w*den/num */
        seq_t bound = (seq_t)w * den / num;   /* floor */
        seq_t x = bound + 1;
        /* skip used denominators */
        int found;
        do {
            found = 0;
            for (size_t i = 0; i < used_cnt; ++i)
                if (used[i] == x) { found = 1; break; }
            if (found) ++x;
        } while (found);

        used[used_cnt++] = x;
        if (x > max_den) max_den = x;

        /* Update R = R - w/x */
        seq_t new_num = num * x - (seq_t)w * den;
        seq_t new_den = den * x;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
        if (num == 0) break;  /* should not happen until w=2 */
    }

    /* Step 3: w = 1 */
    seq_t bound1 = den / num;          /* floor */
    seq_t x1 = bound1 + 1;
    int found;
    do {
        found = 0;
        for (size_t i = 0; i < used_cnt; ++i)
            if (used[i] == x1) { found = 1; break; }
        if (found) ++x1;
    } while (found);
    used[used_cnt++] = x1;
    if (x1 > max_den) max_den = x1;

    seq_t new_num1 = num * x1 - den;   /* w=1 */
    seq_t new_den1 = den * x1;
    seq_t g1 = gcd(new_num1, new_den1);
    num = new_num1 / g1;
    den = new_den1 / g1;               /* now num should be 1 */

    /* Step 4: w = 2 */
    seq_t x2 = den * 2;                /* 2*den satisfies equality */
    do {
        found = 0;
        for (size_t i = 0; i < used_cnt; ++i)
            if (used[i] == x2) { found = 1; break; }
        if (found) ++x2;
    } while (found);
    used[used_cnt++] = x2;
    if (x2 > max_den) max_den = x2;

    /* Final remainder should be zero; we ignore it here. */
    free(used);
    return max_den;
}
