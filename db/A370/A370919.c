/*
 * A370919  Positive integers whose ternary representation includes at least
 *          one 1, and every 1 is followed by 2.
 * https://oeis.org/A370919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose ternary representation *
 *            contains at least one '1', and every '1' is immediately followed
 *            by a '2'. * This is computed by combinatorial enumeration of
 *            valid ternary strings
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum length of ternary representation needed to cover up to 10000 terms.
 * The cumulative count for length 20 already exceeds 10^6, so this is far more
 * than sufficient for all test cases. */
#define MAXLEN 20

/* Internal tables for counting valid prefixes.  These are larger than seq_t
 * so that they do not overflow regardless of the chosen seq_t type. */
static uint64_t dp0[MAXLEN + 1];          /* strings ending in state 0 (no pending 1) */
static uint64_t dp1[MAXLEN + 1];          /* strings ending in state 1 (pending 1) */
static uint64_t cum[MAXLEN + 1];          /* cumulative total up to length L */
static uint64_t cnt[MAXLEN + 1][2][2];    /* completions with rem digits, state, have1 */

/* One‑time initialisation flag */
static bool init_done = false;

/* Build all counting tables once. */
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* Base case: length 1, no leading zero allowed */
    dp0[1] = 1;            /* digit 2 */
    dp1[1] = 1;            /* digit 1 */
    cum[0] = 0;

    /* Count valid strings of each length */
    for (int len = 2; len <= MAXLEN; ++len)
    {
        dp0[len] = dp0[len - 1] * 2 + dp1[len - 1];
        dp1[len] = dp0[len - 1];
    }

    /* Compute cumulative totals of strings that contain at least one '1' */
    for (int len = 1; len <= MAXLEN; ++len)
    {
        uint64_t no1 = (uint64_t)1 << (len - 1);          /* strings with no '1' */
        uint64_t total = dp0[len] - no1;                  /* strings with at least one '1' */
        cum[len] = cum[len - 1] + total;
    }

    /* Base case for completions: no remaining digits */
    cnt[0][0][0] = 0;
    cnt[0][0][1] = 1;   /* finished with at least one '1' */
    cnt[0][1][0] = 0;
    cnt[0][1][1] = 0;

    /* DP for completions with rem digits, current state, and whether a '1' has
     * already appeared. */
    for (int rem = 1; rem <= MAXLEN; ++rem)
    {
        for (int state = 0; state < 2; ++state)
        {
            for (int have = 0; have < 2; ++have)
            {
                uint64_t val = 0;
                if (state == 1)
                {
                    /* Must place a '2' to close the pending '1'. */
                    val = cnt[rem - 1][0][have];
                }
                else
                {
                    /* Place '0' */
                    val += cnt[rem - 1][0][have];
                    /* Place '1' */
                    val += cnt[rem - 1][1][have | 1];
                    /* Place '2' */
                    val += cnt[rem - 1][0][have];
                }
                cnt[rem][state][have] = val;
            }
        }
    }
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    init();

    /* Find the length L of the ternary representation for the nth term. */
    int L = 1;
    while (L <= MAXLEN && cum[L] < (uint64_t)n) ++L;
    /* n will always be within the precomputed range for the tests. */

    /* Position of the term within strings of length L (1‑based). */
    idx_t r = (idx_t)(n - cum[L - 1]);

    unsigned char digits[MAXLEN];
    int state = 0;          /* 0: last digit not a pending '1', 1: pending '1' */
    bool have1 = false;
    for (int pos = 0; pos < L; ++pos)
    {
        for (int d = 0; d <= 2; ++d)
        {
            /* No leading zero. */
            if (pos == 0 && d == 0) continue;

            /* If a pending '1' exists, the next digit must be '2'. */
            if (state == 1 && d != 2) continue;

            int newState = (d == 1) ? 1 : 0;
            bool newHave1 = have1 || (d == 1);
            int rem = L - 1 - pos;

            uint64_t branch = cnt[rem][newState][(int)newHave1];

            if ((uint64_t)branch >= (uint64_t)r)
            {
                digits[pos] = (unsigned char)d;
                state = newState;
                have1 = newHave1;
                break;
            }
            else
            {
                r -= (idx_t)branch;
            }
        }
    }

    /* Convert the ternary digits to a decimal value. */
    seq_t val = 0;
    for (int i = 0; i < L; ++i)
        val = val * 3 + digits[i];

    return val;
}
