/*
 * A349520  Let S_k denote the list of pairs (1,k), (2,k), (3,k), ..., (k,k);
 *          sequence lists the pairs in S_1, S_2, S_3, ...
 * https://oeis.org/A349520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9452, u16 n <= 9452, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A349520.
   For each k, the segment is 1,k,2,k,…,k,k.
   The total number of terms up to segment k is k*(k+1).
   For a given index n (≥1) find the smallest k with k*(k+1)≥n,
   then locate the position within the 2k-term segment. */

seq_t a(idx_t n)
{
    /* Find the smallest k such that k*(k+1) >= n. */
    idx_t low = 1, high = 1;
    /* Increase high until it satisfies the condition. */
    while ((unsigned __int128)high * (high + 1) < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                     /* segment number */

    /* Number of terms before this segment. */
    idx_t prev = (unsigned __int128)(k - 1) * k;

    /* Zero‑based offset within the current segment (range 0 … 2k-1). */
    idx_t offset = n - prev - 1;

    /* Even offset → first component of a pair (i,k); odd offset → k. */
    if (offset & 1U)
        return (seq_t)k;
    else {
        idx_t pair = offset >> 1;       /* 0 … k-1 */
        return (seq_t)(pair + 1);
    }
}
