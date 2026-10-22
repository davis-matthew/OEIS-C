/*
 * A313718  Coordination sequence Gal.6.133.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=5*n - (3*(n/11)+inc), where inc = 0 if
 *            n%11<=4, else if n%11<=7 then n%11-4, else 3
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t div11 = n / 11;
    idx_t rem   = n % 11;

    seq_t inc = 0;
    if (rem > 4) {
        if (rem > 7)
            inc = 3;
        else
            inc = (seq_t)(rem - 4);
    }

    seq_t d   = (seq_t)3 * (seq_t)div11 + inc;
    seq_t val = (seq_t)5 * (seq_t)n - d;
    return val;
}
