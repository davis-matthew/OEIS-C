/*
 * A332052  Number of binary words of length n with an even number of
 *          occurrences of the subword 0101.
 * https://oeis.org/A332052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Finite‑automaton DP. The DFA tracks the longest suffix of the
 *            current word that matches a prefix of the pattern 0101 and the
 *            parity (even/odd) of the number of pattern occurrences. The
 *            transition table (states 0..4, bits 0/1) is precomputed. After
 *            processing n characters the answer is the sum of...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Transition table: next[state][bit] -> new matched‑prefix length */
    static const int next[5][2] = {
        {1,0},   /* state 0 : no matched prefix */
        {1,2},   /* state 1 : matched "0"   */
        {3,0},   /* state 2 : matched "01"  */
        {1,4},   /* state 3 : matched "010" */
        {3,0}    /* state 4 : matched "0101" (full match) */
    };

    /* dp[parity][state] - number of words of current length with given
       parity and DFA state.  parity 0 = even, 1 = odd. */
    seq_t dp[2][5] = {0};
    dp[0][0] = (seq_t)1;            /* empty word */

    for (idx_t i = 0; i < n; ++i) {
        seq_t ndp[2][5] = {0};

        for (int parity = 0; parity < 2; ++parity) {
            for (int state = 0; state < 5; ++state) {
                seq_t val = dp[parity][state];
                if (!val) continue;

                /* Append bit 0 */
                int ns0 = next[state][0];
                int np0 = parity ^ (ns0 == 4);
                ndp[np0][ns0] += val;

                /* Append bit 1 */
                int ns1 = next[state][1];
                int np1 = parity ^ (ns1 == 4);
                ndp[np1][ns1] += val;
            }
        }

        /* Move ndp back into dp */
        for (int parity = 0; parity < 2; ++parity)
            for (int state = 0; state < 5; ++state)
                dp[parity][state] = ndp[parity][state];
    }

    /* Sum over all even‑parity states */
    seq_t result = 0;
    for (int state = 0; state < 5; ++state)
        result += dp[0][state];

    return result;
}
