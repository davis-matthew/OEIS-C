/*
 * A317781  Number of binary words of length n containing exactly two
 *          (possibly overlapping) occurrences of the subword 01101.
 * https://oeis.org/A317781
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count binary words of length n containing exactly two (possibly
 *            overlapping) occurrences of the subword 01101. The algorithm
 *            builds a KMP‑style automaton for the pattern and performs
 *            dynamic programming over the number of matches (0,1,2) and the
 *            automaton state (0…4)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 34, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* pattern 01101 */
static const int pattern[5] = {0, 1, 1, 0, 1};
static const int M = 5;          /* pattern length */

/* Failure function for the pattern (KMP).  Computed only once. */
static int pi[5];

/* Compute the failure function pi[0..M-1]. */
static void compute_pi(void)
{
    pi[0] = 0;
    for (idx_t i = 1; i < (idx_t)M; ++i)
    {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j])
            j = pi[j - 1];
        if (pattern[i] == pattern[j])
            ++j;
        pi[i] = j;
    }
}

/* Returns a(n) for n >= 8.  For n < 8 the return value is irrelevant. */
seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    if (pi[0] == 0)            /* compute pi on first use */
        compute_pi();

    /* dp[k][s] = number of words of current length with
       k matches and automaton in state s (0 <= k <= 2, 0 <= s < M). */
    seq_t dp[3][5] = {{0}};
    seq_t ndp[3][5];

    dp[0][0] = 1;               /* empty word: 0 matches, state 0 */

    for (idx_t step = 1; step <= n; ++step)
    {
        /* zero out ndp */
        for (int k = 0; k < 3; ++k)
            for (int s = 0; s < M; ++s)
                ndp[k][s] = 0;

        for (int k = 0; k < 3; ++k)
        {
            for (int s = 0; s < M; ++s)
            {
                seq_t val = dp[k][s];
                if (val == 0)
                    continue;

                for (int b = 0; b < 2; ++b)   /* append bit b */
                {
                    int ns = s;
                    /* KMP state transition */
                    while (ns > 0 && pattern[ns] != b)
                        ns = pi[ns - 1];
                    if (pattern[ns] == b)
                        ++ns;

                    int nk = k;
                    if (ns == M)          /* a full match of the pattern */
                    {
                        ++nk;
                        if (nk > 2)        /* ignore more than 2 matches */
                            continue;
                        ns = pi[M - 1];   /* restart at longest proper prefix */
                    }
                    ndp[nk][ns] += val;   /* accumulate count */
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (int k = 0; k < 3; ++k)
            for (int s = 0; s < M; ++s)
                dp[k][s] = ndp[k][s];
    }

    /* Sum all ending states with exactly two matches. */
    seq_t result = 0;
    for (int s = 0; s < M; ++s)
        result += dp[2][s];

    return result;
}
