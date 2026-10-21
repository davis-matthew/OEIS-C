/*
 * A281552  Write n in the Elias gamma code and sum the positions where there
 *          is a '1' followed immediately to the right by a '0', counting the
 *          leftmost digit as position 1.
 * https://oeis.org/A281552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of positions i of the pattern '10' in the Elias
 *            γ‑code of n (leftmost digit has position 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* not defined for n < 1, but safe guard */

    /* Compute k = floor(log2(n)) */
    idx_t temp = n;
    idx_t k = 0;
    while (temp > 1) {
        temp >>= 1;
        ++k;
    }

    /* low = n - 2^k   (0 ≤ low < 2^k) */
    idx_t pow2k = ((idx_t)1) << k;
    idx_t low = n - pow2k;

    seq_t res = 0;

    /* Pattern that starts at the boundary between the k ones and the
       (k+1)-bit suffix.  It exists iff the most significant bit of low is 0. */
    if (k > 0 && (((low >> k) & 1) == 0))
        res += (seq_t)k;            /* position k */

    /* Scan the suffix of length k+1 to find '10' transitions.
       For j = 1..k let s[j] be the j‑th bit (most significant first)
       of the (k+1)-bit representation of low.  A pattern occurs when
       s[j] == 1 and s[j+1] == 0; its start position in the code is (k + j). */
    for (idx_t j = 1; j <= k; ++j) {
        idx_t shift_j    = k + 1 - j;          /* position of s[j]  */
        idx_t shift_next = shift_j - 1;        /* position of s[j+1] */
        uint64_t bj      = (low >> shift_j)    & 1;
        uint64_t bj_next = (low >> shift_next) & 1;
        if (bj && !bj_next)
            res += (seq_t)(k + j);
    }

    return res;
}
