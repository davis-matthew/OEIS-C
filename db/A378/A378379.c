/*
 * A378379  Minimal x such that there is a partition of (x, x) into sums of
 *          distinct pairs of nonnegative integers with size at least n,
 *          excluding (0, 0).
 * https://oeis.org/A378379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal x such that (x,x) can be partitioned into at
 *            least n distinct ordered non‑zero pairs (a,b). The sequence
 *            satisfies a(m_k) = k*(k+1)*(k+2)/6 where m_k = k*(k+3)/2 (k ≥
 *            1). For general n, a(n) is obtained by taking all pairs with
 *            total sum ≤ T (T minimal with T*(T+3)/2 ≥ n) and remov...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;           /* special case – only pair (1,1) works       */

    /* find minimal T such that T*(T+3)/2 >= n  (T <= sqrt(2n)+1) */
    idx_t T = 0;
    while ((T * (T + 3)) / 2 < n) {
        T++;
    }

    /* sum of a‑values for all pairs with total sum ≤ T */
    seq_t ST = (seq_t)T * (T + 1) * (T + 2) / 6;

    idx_t mT = (T * (T + 3)) / 2;           /* total pairs up to T            */
    idx_t remove = mT - n;                  /* how many directed pairs to delete */

    if (remove == 0) return ST;            /* exact number of pairs             */

    seq_t removedSum = 0;
    idx_t rem = remove;

    for (idx_t j = T; j > 0 && rem > 0; --j) {
        /* how many distinct unordered pairs are available at sum j ?
           each such pair corresponds to two directed pairs          */
        idx_t distinctAvail = j / 2;

        /* take as many distinct pairs as possible, respecting the remaining count */
        idx_t take = (rem / 2 < distinctAvail) ? (rem / 2) : distinctAvail;
        removedSum += (seq_t)take * j;
        rem -= take * 2;

        /* if one pair still needed and sum j is even, take the equal pair */
        if (rem > 0 && (j & 1) == 0) {
            removedSum += (seq_t)(j / 2);
            rem -= 1;
        }
    }

    /* the minimal x is the total sum minus the removed part */
    return ST - removedSum;
}
