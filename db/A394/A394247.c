/*
 * A394247  Maximum number of parts i with multiplicity i possible in a
 *          partition of n.
 * https://oeis.org/A394247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum k such that k(k+1)(2k+1)/6 <= n and (n -
 *            k(k+1)(2k+1)/6) == 0 or (n - k(k+1)(2k+1)/6) > k (This condition
 *            is equivalent to the OEIS description for A394247.)
 * Range:     u8 n <= 1735, u16 n <= 9955, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 0;          /* candidate number of parts i */
    idx_t best_k = 0;     /* best k found so far */
    while (1)
    {
        /* Compute S(k) = k*(k+1)*(2*k+1)/6  with seq_t arithmetic */
        seq_t k_seq = (seq_t)k;
        seq_t S_k = k_seq * (k_seq + 1) * (2 * k_seq + 1) / 6;

        /* Stop if S(k) exceeds n (since S increases with k) */
        if (S_k > (seq_t)n)
            break;

        /* Check feasibility of using k parts i with multiplicity i */
        seq_t rem = (seq_t)n - S_k;
        if (rem == 0 || rem > k_seq)
            best_k = k;

        ++k;
    }

    return (seq_t)best_k;
}
