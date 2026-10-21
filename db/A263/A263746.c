/*
 * A263746  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and every three consecutive
 *          elements having its maximum within 5 of its minimum.
 * https://oeis.org/A263746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A263746
 *
 * Count the number of permutations p of 0..n-1 with
 *   |p(i)-i| <= 2                           (displacement at most 2)
 * and such that for every three consecutive positions
 *   max{p(i),p(i+1),p(i+2)} - min{p(i),p(i+1),p(i+2)} <= 5.
 *
 * Dynamic programming over positions is used.  At step i (0‑based)
 * the state consists of:
 *   - mask (5 bits): which of the values {i-2,i-1,i,i+1,i+2} have
 *     already been used.  Bit j (0..4) corresponds to value i-2+j.
 *   - two offsets (d1,d2) ∈ {−2,…,2}∪{U} describing the values
 *     chosen for positions i-1 and i-2.  U denotes ‘undefined’
 *     (used for the first two positions).  Offsets are
 *     encoded as 0..4 where offset k maps to code k+2; 5 represents
 *     U.  Hence there are 6×6=36 possible (d1,d2) pairs.
 *
 * Transition:
 *   • Choose an unused value v = i-2+j (bit j is 0).
 *   • If i ≥ 2 the window constraint is checked with the two
 *     previous values.  If it is violated, the transition is rejected.
 *   • Update the mask:  newMask = (mask | (1<<j)) >> 1;
 *     (right shift removes the left‑most bit).  The new right
 *     bit (value i+3) is set to 1 if i+3 is outside [0,n-1];
 *     otherwise it stays 0.
 *   • New offsets:  nd1 = j (offset of v relative to i),
 *     nd2 = d1 (offset of previous value relative to i-1).
 *
 * After processing all positions, the answer is the sum of all
 * DP values of the final step.
 */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return 0;

    /* number of masks = 2^5 = 32, number of offset pairs = 6*6 = 36 */
    const int NUM_MASKS = 32;
    const int NUM_STATE = 36;

    /* DP arrays */
    seq_t dp_cur[NUM_MASKS][NUM_STATE];
    seq_t dp_next[NUM_MASKS][NUM_STATE];

    /* helper for offset encoding: 0..4 represents −2..+2, 5 = undefined */
    const int UNDEF = 5;

    /* clear DP arrays */
    for (int m = 0; m < NUM_MASKS; ++m)
        for (int s = 0; s < NUM_STATE; ++s)
            dp_cur[m][s] = 0;

    /* initial mask for position 0 */
    unsigned int initMask = 0;
    for (int j = 0; j < 5; ++j) {
        int val = (int)0 - 2 + j; /* value at position 0 + offset j */
        if (val < 0 || (idx_t)val >= n)
            initMask |= (1u << j); /* mark out-of-range as used */
    }
    /* initial state: both offsets undefined */
    int initState = UNDEF * 6 + UNDEF;
    dp_cur[initMask][initState] = (seq_t)1;

    /* DP over positions */
    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear next DP */
        for (int m = 0; m < NUM_MASKS; ++m)
            for (int s = 0; s < NUM_STATE; ++s)
                dp_next[m][s] = 0;

        /* loop over all current states */
        for (int mask = 0; mask < NUM_MASKS; ++mask) {
            for (int state = 0; state < NUM_STATE; ++state) {
                seq_t ways = dp_cur[mask][state];
                if (!ways) continue;

                int d1 = state / 6;       /* offset for position pos-1 */
                int d2 = state % 6;       /* offset for position pos-2 */

                /* try all possible values at position pos */
                for (int j = 0; j < 5; ++j) {
                    if (mask & (1u << j)) continue;   /* already used */

                    int value = (int)pos - 2 + j;      /* actual value */
                    int offset = j - 2;                /* relative to pos */

                    /* window constraint: only when we have two previous values */
                    if ((pos >= 2) && (d1 < UNDEF) && (d2 < UNDEF)) {
                        int prev1 = (int)(pos - 1) + (d1 - 2);
                        int prev2 = (int)(pos - 2) + (d2 - 2);
                        int mx = value;
                        if (prev1 > mx) mx = prev1;
                        if (prev2 > mx) mx = prev2;
                        int mn = value;
                        if (prev1 < mn) mn = prev1;
                        if (prev2 < mn) mn = prev2;
                        if ((mx - mn) > 5) continue;     /* violates constraint */
                    }

                    /* update mask */
                    unsigned int used = mask | (1u << j);
                    unsigned int newMask = used >> 1;   /* shift right, drop bit 0 */

                    /* set rightmost bit for value pos+3 */
                    if ((pos + 3) >= n)
                        newMask |= (1u << 4);           /* out-of-range -> used */
                    /* else leave bit 4 as 0 (value pos+3 not yet used) */

                    /* new offsets for next state */
                    int nd1 = j;          /* offset of current value relative to pos */
                    int nd2 = d1;         /* previous offset stays the same */

                    int newState = nd1 * 6 + nd2;

                    dp_next[newMask][newState] += ways;
                }
            }
        }

        /* swap DP arrays */
        for (int m = 0; m < NUM_MASKS; ++m)
            for (int s = 0; s < NUM_STATE; ++s)
                dp_cur[m][s] = dp_next[m][s];
    }

    /* sum all final DP entries – this is a(n) */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < NUM_MASKS; ++mask)
        for (int state = 0; state < NUM_STATE; ++state)
            result += dp_cur[mask][state];

    return result;
}
