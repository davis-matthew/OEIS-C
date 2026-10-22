/*
 * A343049  The k-th binary digit of a(n) is the most frequent digit among the
 *          first k binary digits of n (in case of a tie, take the k-th binary
 *          digit of n).
 * https://oeis.org/A343049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The k‑th binary digit of a(n) is the most frequent digit among the first k binary
   digits of n (in case of a tie, take the k‑th binary digit of n).  The computation
   stops before a leading zero would cause the number of zeros to exceed the number
   of ones.  The sequence starts with a(0)=0. */
seq_t a(idx_t n)
{
    /* Special case n = 0. */
    if (n == 0)
        return 0;

    /* Determine the bit length of n. */
    idx_t bitlen = 0;
    for (idx_t tmp = n; tmp; ++bitlen)
        tmp >>= 1;

    seq_t a_val = 0;
    idx_t ones  = 0;
    idx_t zeros = 0;

    for (idx_t bitpos = 0; ; ++bitpos) {
        int bit;
        if (bitpos < bitlen) {
            /* Existing bit of n. */
            bit = (int)((n >> bitpos) & 1ULL);
        } else {
            /* Leading zero – only include if it would not make zeros > ones. */
            if (zeros + 1 > ones)
                break;
            bit = 0;
        }

        /* Update counts. */
        if (bit)
            ++ones;
        else
            ++zeros;

        /* Determine the bit to set in a(n). */
        int a_bit;
        if (ones > zeros)
            a_bit = 1;
        else if (zeros > ones)
            a_bit = 0;
        else /* tie */
            a_bit = bit;

        /* Set the bit if it is 1 and within the width of seq_t. */
        if (a_bit && bitpos < 8 * sizeof(seq_t))
            a_val |= ((seq_t)1 << bitpos);
    }

    return a_val;
}
