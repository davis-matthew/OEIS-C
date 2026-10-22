/*
 * A342101  Remove middle term and append, starting with [1, 2, 3].
 * https://oeis.org/A342101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9690, u16 n <= 9690, u32 n <= 9690, u64 n <= 9690,
 *            u128 n <= 9690, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342101: start from [1,2,3], then repeatedly append the
   current sequence with the same sequence but having its middle
   element removed.  This function returns a(n) for n ≥ 1 without
   constructing the entire sequence. */

seq_t a(idx_t n) {
    /* n is 1‑based.  For n == 0 the function returns 0. */
    if (n == 0) return (seq_t)0;
    /* Base values for the initial sequence [1,2,3]. */
    if (n <= 3) return (seq_t)(n);  /* 1 → 1, 2 → 2, 3 → 3 */

    /* Find the smallest level k with length L_k = 2^{k+1}+1 ≥ n. */
    idx_t len = 3;                     /* L_0 */
    while (len < n) {
        /* L_{k+1} = 2*L_k - 1  (no overflow for typical n). */
        len = (len << 1) - 1;
    }

    /* Work backwards from level k to level 0. */
    while (n > 3) {
        /* Previous level length: L_{k-1} = (len + 1)/2. */
        idx_t lenprev = (len + 1) >> 1;
        /* Middle position (0‑based) in the previous sequence. */
        idx_t midprev = lenprev >> 1;   /* floor(lenprev/2) */

        if (n <= lenprev) {
            /* The element lies in the unchanged prefix. */
            len = lenprev;
            continue;
        }

        /* The element lies in the appended part of size lenprev - 1. */
        n -= lenprev;           /* 1‑based index inside the appended part */

        if (n <= midprev) {
            /* It maps to the same position before the centre. */
            /* n stays unchanged. */
        } else {
            /* It comes from after the centre, so indices are shifted by +1. */
            n += 1;
        }

        /* Move to the previous level. */
        len = lenprev;
    }

    /* n is now 1, 2, or 3.  Return the corresponding base value. */
    return (seq_t)(n);
}
