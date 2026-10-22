/*
 * A393980  On the nonnegative integer number line, go to the first next
 *          unused power of 2 minus 1, then spiral back to pick up any skipped
 *          numbers, and repeat.
 * https://oeis.org/A393980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(0) = 0; For n >= 1, let k = floor(log₂(n)), i.e. 2^k ≤ n ≤
 *            2^(k+1) – 1. Let offset = n – 2^k. If offset is even: a(n) =
 *            2^(k+1) – 1 – offset/2. If offset is odd : a(n) = 2^k + (offset
 *            – 1)/2. This generates each block [2^k .. 2^(k+1) – 1] as
 *            2^(k+1)–1, 2^k, 2^(k+1)–2, 2^k+1, …
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)0;

    /* compute k = floor(log₂(n)) */
    idx_t k = 0;
    idx_t tmp = n;
    while ((tmp >>= 1) != 0)
        ++k;

    /* offset within the block */
    idx_t offset = n - ((idx_t)1 << k);

    if ((offset & 1) == 0) {
        /* even offset: use high side */
        seq_t high = ((seq_t)1ULL << (k + 1)) - 1;   /* 2^(k+1) – 1 */
        seq_t half = (seq_t)(offset >> 1);           /* offset / 2 */
        return high - half;
    } else {
        /* odd offset: use low side */
        seq_t low  = ((seq_t)1ULL << k);             /* 2^k */
        seq_t half = (seq_t)((offset - 1) >> 1);     /* (offset – 1) / 2 */
        return low + half;
    }
}
