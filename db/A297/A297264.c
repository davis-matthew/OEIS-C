/*
 * A297264  Numbers whose base-8 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑8 digits have a
 *            greater down‑variation than up‑variation
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 8;           /* first candidate */
    idx_t count = 0;

    while (1)
    {
        /* compute DV and UV for cur */
        seq_t num  = cur;
        seq_t prev = num % 8;
        num = num / 8;
        seq_t dv = 0;
        seq_t uv = 0;

        while (num > 0)
        {
            seq_t d = num % 8;
            if (d > prev)
                dv += d - prev;
            else if (d < prev)
                uv += prev - d;
            prev = d;
            num = num / 8;
        }

        if (dv > uv)
        {
            ++count;
            if (count == n)
                return cur;
        }

        ++cur;   /* next integer */
    }
}
