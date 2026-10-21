/*
 * A241960  Number of length n+3 0..4 arrays with no consecutive four elements
 *          summing to more than 2*4.
 * https://oeis.org/A241960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A241960:  a(n) = number of 0..4 arrays of length n+3
 * such that every 4 consecutive elements sum to ≤ 8.
 * 
 * Computation is performed by a simple DP over the last 3 elements.
 * State index: (a,b,c) for the last 3 elements, encoded as a*25+b*5+c (0≤a,b,c≤4).
 * Transition: for each state (a,b,c) and each possible d (0..4) with a+b+c+d≤8,
 *    new_state = (b,c,d) and dp_next[ new_state ] += dp_cur[ a,b,c ].
 * Start with all 125 triples at step 3 (no constraint yet). Traverse
 * up to length n+3, summing all final states.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* sequence offset is 1, behaviour for n<1 irrelevant */

    idx_t L = n + 3;                   /* total length of array */
    seq_t prev[125];                  /* counts for step pos-1 */
    seq_t cur [125];                  /* counts for step pos  */

    /* Base: length 3, all 125 triples are allowed */
    for (int i = 0; i < 125; ++i) prev[i] = 1;

    /* Dynamic programming from length 4 up to L */
    for (idx_t pos = 4; pos <= L; ++pos) {
        for (int i = 0; i < 125; ++i) cur[i] = 0;

        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                for (int c = 0; c < 5; ++c) {
                    int idx = a * 25 + b * 5 + c;
                    seq_t val = prev[idx];
                    if (val == 0) continue;

                    int sum3 = a + b + c;
                    for (int d = 0; d < 5; ++d) {
                        if (sum3 + d <= 8) {
                            int next = b * 25 + c * 5 + d;
                            cur[next] += val;
                        }
                    }
                }

        /* Prepare for next iteration */
        for (int i = 0; i < 125; ++i) prev[i] = cur[i];
    }

    /* Sum all states of the final length */
    seq_t result = 0;
    for (int i = 0; i < 125; ++i) result += prev[i];
    return result;
}
