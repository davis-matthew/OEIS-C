/*
 * A393803  Numbers whose number of nonzero digits is identical in base 2 and
 *          3.
 * https://oeis.org/A393803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer x (in increasing order) such that the
 *            number of non‑zero digits in base 2 equals that in base 3
 * Range:     u8 n <= 67, u16 n <= 9886, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t   seen = 0;          /* how many qualifying numbers found */
    seq_t   cur  = 0;          /* current integer being tested */

    for (;;)
    {
        /* Count 1‑bits in binary representation of cur. */
        seq_t t = cur;
        seq_t ones = 0;
        while (t)
        {
            if (t & 1) ++ones;
            t >>= 1;
        }

        /* Count non‑zero digits in ternary representation of cur. */
        t = cur;
        idx_t nz = 0;
        while (t)
        {
            seq_t r = t % 3;
            if (r) ++nz;
            t /= 3;
        }

        /* If the counts match, record the value. */
        if (ones == (seq_t)nz)
        {
            ++seen;
            if (seen == n) return cur;
        }

        /* Move to the next integer. */
        ++cur;
    }
}
