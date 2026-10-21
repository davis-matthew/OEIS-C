/*
 * A257069  Number of length 7 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A257069: Number of length‑7 arrays with entries 1..(n+1) whose every
 * leading partial sum is divisible by 2 or 3.  We count by dynamic
 * programming on the 6 residue classes modulo 6,
 * forbidding residues 1 and 5 because 2∤S and 3∤S cannot be true
 * only when S mod 6 ∈ {1,5}.
 *
 * The recurrence is independent of n.
 */

seq_t a(idx_t n)
{
    /* Allowed residues for a partial sum mod 6 */
    static const int allowed[6] = {1, 0, 1, 1, 1, 0};

    idx_t N = n + 1;                /* values 1..N */

    /* Count of numbers with each residue mod 6 */
    seq_t count[6];
    for (int r = 0; r < 6; ++r) {
        if (r == 0) {                        /* residue 0 : 6,12,... */
            count[0] = (N >= 6) ? (seq_t)(N / 6) : 0;
        } else {
            if (N >= (idx_t)r)
                count[r] = (seq_t)((N - r) / 6 + 1);
            else
                count[r] = 0;
        }
    }

    /* DP over the 6 residues; dp[res] = number of ways */
    seq_t dp[6] = {0};
    dp[0] = 1;                              /* start from sum 0 */

    /* Seven positions */
    for (int step = 0; step < 7; ++step) {
        seq_t next[6] = {0};
        for (int res_cur = 0; res_cur < 6; ++res_cur) {
            if (dp[res_cur] == 0) continue;
            for (int r_i = 0; r_i < 6; ++r_i) {
                if (count[r_i] == 0) continue;
                int res_next = (res_cur + r_i) % 6;
                if (!allowed[res_next]) continue;
                next[res_next] = next[res_next] + dp[res_cur] * count[r_i];
            }
        }
        for (int r = 0; r < 6; ++r) dp[r] = next[r];
    }

    /* Sum over all residues – only allowed ones survive */
    seq_t total = 0;
    for (int r = 0; r < 6; ++r) total += dp[r];
    return total;
}
