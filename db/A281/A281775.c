/*
 * A281775  Number of distinct topologies on an n-set that have exactly 7 open
 *          sets.
 * https://oeis.org/A281775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*Stirling2(n,4)+240*Stirling2(n,5)+720*Stirling2(n,6)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence S(n,k) = k*S(n‑1,k) + S(n‑1,k‑1) for k=1…6. */
seq_t a(idx_t n)
{
    /* s[k] holds S(current_n, k)   (k=0…6) */
    seq_t s[7] = {0};
    seq_t next[7];
    s[0] = (seq_t)1;                     /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        next[0] = (seq_t)0;
        for (int k = 1; k <= 6; ++k) {
            next[k] = s[k] * (seq_t)k + s[k - 1];
        }
        for (int k = 0; k <= 6; ++k) {
            s[k] = next[k];
        }
    }

    return (seq_t)54 * s[4] + (seq_t)240 * s[5] + (seq_t)720 * s[6];
}
