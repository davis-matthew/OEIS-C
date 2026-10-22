/*
 * A400891  Numbers k such that the 2-adic valuation of 3*k+2 is even.
 * https://oeis.org/A400891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the 2-adic valuation of 3*k+2 is even (OEIS A400891) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but defensive */

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* compute 3*k + 2 in seq_t */
        seq_t tmp = (seq_t)3 * k + 2;

        /* count trailing zeros of tmp – i.e., the 2-adic valuation of 3*k+2 */
        uint64_t v = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++v;
        }

        /* even valuation */
        if ((v & 1) == 0) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* k is the nth term of the sequence */
        }
    }
}
