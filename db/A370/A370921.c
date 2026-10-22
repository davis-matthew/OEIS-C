/*
 * A370921  Positive integers in whose ternary representation, 2 occurs at
 *          least once, and every 2 is followed by 1.
 * https://oeis.org/A370921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose ternary representation
 *            contains at least one digit 2 and every digit 2 is immediately
 *            followed by a 1 (i.e. the ternary string never contains 20 or 22
 *            and never ends in 2)
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAXLEN 128   /* sufficient for n up to many thousands */

static unsigned long long dp[MAXLEN + 1][2];

/* Compute dp[pos][have2] for strings of total length L */
static void compute_dp(idx_t L)
{
    dp[L][0] = 0;          /* no 2 seen yet */
    dp[L][1] = 1;          /* at least one 2 seen, finished */
    for (int pos = (int)L - 1; pos >= 0; --pos) {
        for (int have2 = 0; have2 <= 1; ++have2) {
            unsigned long long val = 0;
            /* choose digit 0 */
            val += dp[pos + 1][have2];
            /* choose digit 1 */
            val += dp[pos + 1][have2];
            /* choose digit 2 (must leave room for a following 1) */
            if (L - pos >= 2)
                val += dp[pos + 2][1];
            dp[pos][have2] = val;
        }
    }
}

/* Return the kth lexicographic string of length L (1‑based). */
static seq_t kth_value(idx_t L, unsigned long long k)
{
    seq_t value = 0;
    int pos = 0;
    bool have2 = false;

    while (pos < (int)L) {
        int digits[3];
        int ndigits;
        if (pos == 0) {            /* most significant digit cannot be 0 */
            digits[0] = 1;
            digits[1] = 2;
            ndigits = 2;
        } else {
            digits[0] = 0;
            digits[1] = 1;
            digits[2] = 2;
            ndigits = 3;
        }

        /* iterate possible digits in ascending order */
        for (int idx = 0; idx < ndigits; ++idx) {
            int d = digits[idx];
            unsigned long long cnt = 0;

            if (d == 2 && L - pos >= 2)
                cnt = dp[pos + 2][1];
            else if (d != 2)
                cnt = dp[pos + 1][have2];
            else
                cnt = 0; /* cannot place 2 at last position */

            if (k > cnt) {
                k -= cnt;
                continue;
            }

            /* choose this digit */
            value = value * (seq_t)3 + (seq_t)d;
            if (d == 2) {
                /* forced following 1 */
                value = value * (seq_t)3 + (seq_t)1;
                pos += 2;
                have2 = true;
            } else {
                pos += 1;
            }
            break;
        }
    }
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    unsigned long long cumulative = 0;
    idx_t len = 2;

    /* find the length of the n-th term */
    while (1) {
        compute_dp(len);
        unsigned long long count = dp[1][0];            /* first digit = 1 */
        if (len >= 2)                                  /* first digit = 2 */
            count += dp[2][1];
        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++len;
        if (len > MAXLEN)        /* safety guard, should not happen for tested n */
            break;
    }

    unsigned long long k = n - cumulative;             /* 1‑based index within this length */
    compute_dp(len);
    return kth_value(len, k);
}
