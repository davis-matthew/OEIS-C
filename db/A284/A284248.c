/*
 * A284248  Every binary string w of length n has a subword of length a(n)
 *          that appears at least twice in w.
 * https://oeis.org/A284248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = largest j such that 2^j + j <= n (OEIS A284248)
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the value would not be queried. */
    if (n == 0)
        return (seq_t)0;

    idx_t    j  = 0;          /* current candidate j */
    seq_t    pow2 = (seq_t)1; /* current power of two: 2^j */
    seq_t    sn   = (seq_t)n; /* cast to seq_t for mixed operations */

    /* Increment j while the inequality holds. */
    while (pow2 + (seq_t)j <= sn) {
        j++; /* we have found a valid j     */

        /* If the next doubling would overflow or certainly exceed n,
           there is no point in continuing. */
        if (pow2 > sn / 2)
            break;

        pow2 <<= 1; /* move to 2^(j+1) */
    }

    /* The loop stops after counting one too many; subtract to get result. */
    return (seq_t)(j - 1);
}
