/*
 * A248327  Levenshtein distance of n and its reversal in decimal
 *          representation, cf. A004086.
 * https://oeis.org/A248327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Levenshtein distance between the decimal representation
 *            of n and the decimal representation of rev(n) (with leading
 *            zeros removed)
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/rev.h"          /* for rev(idx_t)          */
#include "../utils/get_digits.h"   /* for get_digits(seq_t , ... ) */

seq_t a(idx_t n)
{
    /* extract decimal digits of n */
    uint8_t a_digits[21];
    size_t a_len = 0;
    get_digits((seq_t)n, a_digits, &a_len);      /* a_digits[0] is least significant */

    /* reverse n to obtain rev(n) */
    idx_t r_int = rev(n);
    seq_t rev_val = (seq_t)r_int;

    /* extract decimal digits of rev(n) */
    uint8_t r_digits[21];
    size_t r_len = 0;
    get_digits(rev_val, r_digits, &r_len);

    /* Levenshtein DP: dp[i][j] distance between first i digits of a and first j digits of rev */
    /* dp array is small (max 20x20), so static allocation is fine */
    unsigned int dp[21][21] = {0};

    for (size_t i = 1; i <= a_len; ++i)
        dp[i][0] = (unsigned int)i;          /* delete i digits */

    for (size_t j = 1; j <= r_len; ++j)
        dp[0][j] = (unsigned int)j;          /* insert j digits */

    for (size_t i = 1; i <= a_len; ++i)
    {
        for (size_t j = 1; j <= r_len; ++j)
        {
            unsigned int replace_cost = (a_digits[i-1] == r_digits[j-1])
                                        ? dp[i-1][j-1]
                                        : dp[i-1][j-1] + 1;
            unsigned int delete_cost  = dp[i-1][j] + 1;    /* delete a[i-1]   */
            unsigned int insert_cost  = dp[i][j-1] + 1;    /* insert r[j-1]   */

            unsigned int best = replace_cost;
            if (delete_cost < best) best = delete_cost;
            if (insert_cost < best) best = insert_cost;

            dp[i][j] = best;
        }
    }

    return (seq_t)dp[a_len][r_len];
}
