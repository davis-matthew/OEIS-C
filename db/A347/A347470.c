/*
 * A347470  Least product of any two numbers whose concatenation is n,
 *          excluding 0*n for n > 9.
 * https://oeis.org/A347470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least product of two numbers whose decimal concatenation
 * is n, with no leading zeros allowed in either part.  The trivial
 * decomposition n = concat(0,n) cannot occur for n>9 because the
 * concatenation would start with a zero, so the algorithm automatically
 * excludes it.
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to its decimal string representation */
    char s[21]; /* uint64_t max has 20 decimal digits + null */
    int len = snprintf(s, sizeof s, "%" PRIu64, (uint64_t)n);
    if (len <= 1)          /* n < 10 has no valid split */
        return (seq_t)0;

    bool found = false;
    seq_t best = 0;

    for (int pos = 1; pos < len; ++pos) {
        /* Skip splits where either part would have a leading zero */
        if (s[0] == '0' && pos > 1) continue;              /* left part */
        if (s[pos] == '0' && (len - pos) > 1) continue;    /* right part */

        /* Parse left part */
        seq_t left = 0;
        for (int i = 0; i < pos; ++i)
            left = left * (seq_t)10 + (seq_t)(s[i] - '0');

        /* Parse right part */
        seq_t right = 0;
        for (int i = pos; i < len; ++i)
            right = right * (seq_t)10 + (seq_t)(s[i] - '0');

        seq_t prod = left * right;
        if (!found || prod < best) {
            best = prod;
            found = true;
        }
    }

    return found ? best : (seq_t)0;
}
