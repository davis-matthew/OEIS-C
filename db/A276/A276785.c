/*
 * A276785  Number of binary strings of length n containing the substring
 *          11011.
 * https://oeis.org/A276785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Way: a(n) = 2^n - (# of binary strings of length n that avoid the
   substring 11011).  We count avoiders by a linear DP on the
   automaton of the pattern.  Complexity O(n). */

seq_t a(idx_t n)
{
    /* transition table for the pattern '11011':
       states 0..4 are the lengths of the longest prefix of the pattern
       that equals a suffix of the processed part.  Transitions to state 5
       mean the pattern has appeared and we ignore them. */
    static const unsigned next[5][2] = {
        {0,1},   /* state 0 : ""  -> 0 (0), 1 (1) */
        {0,2},   /* state 1 : "1" -> 0 (0), 2 (1) */
        {3,2},   /* state 2 : "11" -> 3 (0), 2 (1) */
        {0,4},   /* state 3 : "110" -> 0 (0), 4 (1) */
        {0,5}    /* state 4 : "1101" -> 0 (0), 5 (1) */
    };

    seq_t total = 1;
    for (idx_t i = 0; i < n; ++i)
        total *= 2;

    /* DP arrays for states 0..4 (state 5 is absorbing and ignored). */
    seq_t cur[5] = {0};
    seq_t nxt[5] = {0};
    cur[0] = 1;  /* empty string */

    for (idx_t len = 0; len < n; ++len) {
        for (int s = 0; s < 5; ++s) nxt[s] = 0;
        for (int s = 0; s < 5; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            /* append 0 */
            int t0 = next[s][0];
            if (t0 < 5) nxt[t0] += val;
            /* append 1 */
            int t1 = next[s][1];
            if (t1 < 5) nxt[t1] += val;
        }
        /* copy nxt to cur for next iteration */
        for (int s = 0; s < 5; ++s) cur[s] = nxt[s];
    }

    seq_t avoid = 0;
    for (int s = 0; s < 5; ++s) avoid += cur[s];

    return total - avoid;
}
