/*
 * A361838  a(n) is the number of 2s in the binary hereditary representation
 *          of 2n.
 * https://oeis.org/A361838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hear
 * Formula:   a(n) = number of 2s in the binary hereditary representation of
 *            2n
 * Range:     u8 n <= 9334, u16 n <= 9334, u32 n <= 9334, u64 n <= 9334,
 *            u128 n <= 9334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper function that counts the 2s in the hereditary representation
   of a positive integer x (x > 0).  The definition follows the
   recursive rule that a term 2^t contributes one 2 (for the base 2)
   and the 2s that appear in the hereditary representation of the
   exponent t.  The term 2^0 (=1) does not contribute any 2. */
static seq_t count_2s(idx_t x)
{
    if (x < 2)
        return 0;

    seq_t total = 0;
    for (idx_t t = 0; x >> t; ++t) {
        if ((x & ((idx_t)1 << t)) && t > 0)
            total += 1 + count_2s(t);
    }
    return total;
}

seq_t a(idx_t n)
{
    /* We compute a(n) = count_2s(2*n) */
    return count_2s(n << 1);
}
