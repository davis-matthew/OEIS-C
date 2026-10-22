/*
 * A384837  a(n) = Sum_{k=0..floor(n/5)} |Stirling1(n,5*k)|.
 * https://oeis.org/A384837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} |Stirling1(n,5k)| = Σ_{k≡0 (mod 5)}
 *            s(n,k) Recurrence: let S[r] = Σ_{k≡r (mod 5)} s(current_n,k).
 *            Then for n→n+1: S_next[r] = S[(r-1) mod 5] + n * S[r] Initial:
 *            S[0]=1, others 0. a(n)=S[0] after n steps
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* S[0..4] are the current sums of Stirling numbers with cycle counts
       congruent to 0..4 modulo 5.  All values are unsigned and may be
       arbitrary-precision integers in the full build. */
    seq_t S[5] = {0};
    S[0] = (seq_t)1;        /* base case: s(0,0)=1 */

    for (idx_t i = 0; i < n; ++i) {
        seq_t next[5];
        /* r-1 modulo 5 is equivalent to (r+4)%5 */
        for (int r = 0; r < 5; ++r) {
            int prev = (r + 4) % 5;
            next[r] = S[prev] + (seq_t)i * S[r];
        }
        /* copy next back into S */
        S[0] = next[0];
        S[1] = next[1];
        S[2] = next[2];
        S[3] = next[3];
        S[4] = next[4];
    }
    return S[0];   /* a(n) is the sum over cycle counts divisible by 5 */
}
