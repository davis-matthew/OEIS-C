/*
 * A285196  If A_k denotes the first 2*3^k terms, then A_0 = 01, A_{k+1} = A_k
 *          A_k B_k, where B_k is the reversal of A_k.
 * https://oeis.org/A285196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  A_0 = 01,  A_{k+1} = A_k A_k complement(A_k) */
/* where complement(A_k) is obtained by interchanging 0's and 1's in A_k. */
seq_t a(idx_t n) {
    /* Find the length L = 2 * 3^k that strictly exceeds n. */
    idx_t L = 2;                     /* length of A_0 */
    while (L <= n) {
        L *= 3;
    }

    /* Map n down to the base case while accumulating complement toggles. */
    idx_t pos = n;                   /* position within the current block  */
    unsigned inv = 0;                /* 0 = no complement, 1 = complement */

    while (L > 2) {                  /* while we still have more than 2 elements */
        idx_t L1 = L / 3;             /* length of the constituent A_k blocks */

        if (pos < L1) {
            /* first occurrence of A_k - nothing to change */
        } else if (pos < 2 * L1) {
            /* second occurrence of A_k - shift index */
            pos -= L1;
        } else {
            /* complement block - shift index and toggle complement */
            pos -= 2 * L1;
            inv ^= 1;
        }

        L = L1;                       /* descend to the lower level */
    }

    /* Base sequence A_0 = "01"  (pos is 0 or 1). */
    return (seq_t)(pos ^ inv);        /* apply complement if necessary */
}
