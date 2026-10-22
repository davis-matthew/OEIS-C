/*
 * A340520  a(n) = 2*A006463(n) + 1.
 * https://oeis.org/A340520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A006463(n) + 1
 * Range:     u8 n <= 30, u16 n <= 1088, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A006463 is constructed by adding blocks where the integer k is repeated k+1 times. */

seq_t a(idx_t n)
{
    /* Number of increments needed to reach a(n) from a(1) */
    idx_t s = n - 1;
    idx_t k = 1;  /* current block (k=1 corresponds to the increment value 1) */

    /* Find the block that contains the s-th increment */
    while ((((seq_t)k * (seq_t)(k + 3)) / 2) < (seq_t)s)
        ++k;

    /* Total increments completed in all previous blocks */
    seq_t prevBlocks = ((seq_t)(k - 1) * (seq_t)(k + 2)) / 2;

    /* Remaining increments needed within the current block */
    seq_t rem = (seq_t)s - prevBlocks;

    /* Sum of all full previous blocks:
       Σ_{i=1}^{k-1} i*(i+1) = (k-1)k(k+1)/3   (exact for integer k) */
    seq_t fullSum = ((seq_t)(k - 1) * (seq_t)k * (seq_t)(k + 1)) / 3;

    /* Current value of A006463(n) */
    seq_t a006 = fullSum + (seq_t)k * rem;

    /* Return a(n) = 2*A006463(n) + 1 */
    return a006 * 2 + 1;
}
