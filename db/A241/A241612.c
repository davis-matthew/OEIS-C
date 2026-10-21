/*
 * A241612  Number of length n+2 0..6 arrays with no consecutive three
 *          elements summing to more than 6.
 * https://oeis.org/A241612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count length‑(n+2) arrays of numbers 0..6 where every three
 *            consecutive elements sum to at most 6. Implemented by dynamic
 *            programming on the last two elements. The DP state is a pair
 *            (i,j) with 0≤i,j≤6. For each pair we add possible next elements
 *            k with i+j+k≤6, updating the pair (j,k). The total...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Length of the array to count: L = n+2 */
    idx_t L = n + 2;
    /* Number of states: 7*7 = 49 */
    const idx_t STATES = 49;
    /* DP arrays for current and next step */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* Initialize for length 2: every pair is possible: count = 1 */
    for (idx_t i = 0; i < 7; ++i)
        for (idx_t j = 0; j < 7; ++j)
            dp_cur[i * 7 + j] = 1;

    /* Perform DP transitions for lengths 3 .. L */
    for (idx_t len = 3; len <= L; ++len)
    {
        /* clear next array */
        for (idx_t idx = 0; idx < STATES; ++idx)
            dp_next[idx] = 0;

        for (idx_t i = 0; i < 7; ++i)
        {
            for (idx_t j = 0; j < 7; ++j)
            {
                idx_t base = i * 7 + j;
                seq_t cur_cnt = dp_cur[base];
                if (!cur_cnt) continue;           /* skip zero count */

                /* possible next value k, maintain i + j + k ≤ 6 */
                for (idx_t k = 0; k < 7; ++k)
                {
                    if (i + j + k > 6) break;      /* sums increase with k */
                    idx_t new_idx = j * 7 + k;
                    dp_next[new_idx] += cur_cnt;
                }
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        for (idx_t idx = 0; idx < STATES; ++idx)
            dp_cur[idx] = dp_next[idx];
    }

    /* Sum over all final states: total number of valid arrays */
    seq_t total = 0;
    for (idx_t idx = 0; idx < STATES; ++idx)
        total += dp_cur[idx];

    return total;
}
