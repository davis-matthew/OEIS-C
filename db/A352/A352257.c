/*
 * A352257  Sum of all parts of all partitions of n into an odd number of
 *          consecutive parts.
 * https://oeis.org/A352257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (# of odd k dividing n with k(k+1)/2 <= n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    idx_t k = 1;
    unsigned int cnt = 0;

    for (;;)
    {
        seq_t part = (seq_t)k * ((seq_t)(k + 1)) / 2;
        if (part > nseq)
            break;
        if (n % k == 0)
            cnt++;
        k += 2;          /* next odd k */
    }

    return nseq * cnt;
}
