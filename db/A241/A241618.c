/*
 * A241618  Number of length n+2 0..12 arrays with no consecutive three
 *          elements summing to more than 12.
 * https://oeis.org/A241618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A241618: # of (n+2)-length arrays with elements 0..12
   where no three consecutive elements have a sum > 12.

   The DP keeps track of the last two elements of the prefix.
   For each prefix of length ℓ (ℓ ≥ 2) we have a state (a,b) meaning
   that the last two elements are a,b.  Adding a new element c is
   allowed iff a+b+c ≤ 12.  The transition updates the state to (b,c).
   The number of valid sequences of length ℓ+1 is obtained by summing
   over all c that satisfy the condition.

   Complexity: O(n·13^3) ≈ 2.2·10^7 operations for n=10 000,
   well below the 2‑second limit.  The implementation works for all
   supported `seq_t` types (native unsigned ints or arbitrary‑precision).
*/

seq_t a(idx_t n)
{
    /* Length of the sequence to construct */
    idx_t L = n + 2;           /* n ≥ 1  →  L ≥ 3 */

    /* DP tables: cur[a][b] = number of prefixes of current length
       ending with (a,b)  (a,b ∈ [0,12]) */
    seq_t cur[13][13];
    seq_t nxt[13][13];

    /* Initialize: all length‑2 prefixes are valid */
    for (idx_t i = 0; i <= 12; ++i)
        for (idx_t j = 0; j <= 12; ++j)
            cur[i][j] = 0;

    for (idx_t i = 0; i <= 12; ++i)
        for (idx_t j = 0; j <= 12; ++j)
            cur[i][j] = 1;   /* each pair (i,j) forms a valid length‑2 prefix */

    /* Build prefixes of increasing length until we reach length L */
    for (idx_t pos = 2; pos < L; ++pos) {
        /* zero the next table */
        for (idx_t i = 0; i <= 12; ++i)
            for (idx_t j = 0; j <= 12; ++j)
                nxt[i][j] = 0;

        /* transition: from state (a,b) add a third element c */
        for (idx_t a = 0; a <= 12; ++a) {
            for (idx_t b = 0; b <= 12; ++b) {
                seq_t count = cur[a][b];
                if (!count) continue;          /* skip zero counts to save work */

                for (idx_t c = 0; c <= 12; ++c) {
                    if (a + b + c <= 12)
                        nxt[b][c] += count;
                }
            }
        }

        /* swap cur and nxt */
        for (idx_t i = 0; i <= 12; ++i)
            for (idx_t j = 0; j <= 12; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Sum over all ending states to get the total number of sequences */
    seq_t result = 0;
    for (idx_t i = 0; i <= 12; ++i)
        for (idx_t j = 0; j <= 12; ++j)
            result += cur[i][j];

    return result;
}
