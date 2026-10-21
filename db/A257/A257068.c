/*
 * A257068  Number of length 6 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 59, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of length-6 arrays over {1,…,n+1} with every
   leading partial sum divisible by 2 or by 3.  The value is obtained
   by a short dynamic-programming routine that iterates over the 6
   positions and the 6 residue classes mod 6. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    seq_t m = (seq_t)(n + 1);              /* range of array entries */

    /* Count how many numbers 1..m fall into each residue class r mod 6   */
    seq_t cnt[6];
    cnt[0] = m / 6;
    for (int r = 1; r <= 5; ++r)
        cnt[r] = (m >= (seq_t)r) ? ((m - (seq_t)r) / 6 + 1) : 0;

    /* dynamic programming over the 6 positions                     */
    seq_t prev[6] = {0}, next[6] = {0};
    prev[0] = 1;                            /* empty prefix sum = 0 */

    for (int step = 1; step <= 6; ++step) {
        for (int i = 0; i < 6; ++i) next[i] = 0;
        for (int p = 0; p < 6; ++p) {
            seq_t ways = prev[p];
            if (ways == 0) continue;
            for (int r = 0; r < 6; ++r) {
                seq_t c = cnt[r];
                if (c == 0) continue;
                int new_r = (p + r) % 6;
                if (new_r == 1 || new_r == 5) continue;  /* not div. 2 or 3 */
                next[new_r] += ways * c;
            }
        }
        for (int i = 0; i < 6; ++i) prev[i] = next[i];
    }

    /* All remaining residues are admissible; sum them up */
    seq_t result = 0;
    for (int i = 0; i < 6; ++i) result += prev[i];
    return result;
}
