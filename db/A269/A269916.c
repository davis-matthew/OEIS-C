/*
 * A269916  Number of ternary strings of length n with maximal run length four
 *          containing 11112.
 * https://oeis.org/A269916
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ternary strings of length n with maximal run
 *            length ≤4 that contain the substring 11112. Implemented with a
 *            DP over an automaton with 30 states (last character, run length,
 *            hasPattern flag)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* DP arrays: curr and next. 
       Dimensions: lastChar in {0,1,2}, runLen in {1..4}, hasPattern in {0,1}.
       Zeroed manually because seq_t may not be trivially zeroable. */
    seq_t curr[3][5][2];
    seq_t next[3][5][2];
    seq_t dp_none = (seq_t)1;           /* initial state: no characters yet */

    /* initialise all entries to zero */
    for (int last = 0; last < 3; ++last)
        for (int run = 1; run <= 4; ++run)
            for (int h = 0; h < 2; ++h)
                curr[last][run][h] = (seq_t)0;

    /* iterate over string positions */
    for (idx_t pos = 0; pos < n; ++pos)
    {
        /* clear next array */
        for (int last = 0; last < 3; ++last)
            for (int run = 1; run <= 4; ++run)
                for (int h = 0; h < 2; ++h)
                    next[last][run][h] = (seq_t)0;

        /* transitions from the initial "none" state (only on first step) */
        for (int s = 0; s < 3; ++s)                 /* new symbol 0,1,2 */
            next[s][1][0] += dp_none;

        /* transitions from existing states */
        for (int last = 0; last < 3; ++last)
        {
            for (int run = 1; run <= 4; ++run)
            {
                for (int has = 0; has < 2; ++has)
                {
                    seq_t val = curr[last][run][has];
                    if (val == (seq_t)0)
                        continue;

                    for (int s = 0; s < 3; ++s)
                    {
                        int newRun = (s == last) ? run + 1 : 1;
                        if (newRun > 4)
                            continue;

                        int newHas = has || (s == 2 && last == 1 && run == 4);
                        next[s][newRun][newHas] += val;
                    }
                }
            }
        }

        /* copy next into curr for next iteration */
        for (int last = 0; last < 3; ++last)
            for (int run = 1; run <= 4; ++run)
                for (int h = 0; h < 2; ++h)
                    curr[last][run][h] = next[last][run][h];

        /* after the first iteration, the "none" state no longer exists */
        dp_none = (seq_t)0;
    }

    /* sum all states that have matched the pattern */
    seq_t ans = (seq_t)0;
    for (int last = 0; last < 3; ++last)
        for (int run = 1; run <= 4; ++run)
            ans += curr[last][run][1];

    return ans;
}
