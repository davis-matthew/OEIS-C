/*
 * A313487  Coordination sequence Gal.4.42.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(2)=10; for n>=3,
   a(n) = a(n-1) + 4 if n≡3,4,5 (mod 7), else +5. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 5;
    if (n == 2)
        return 10;

    seq_t result = 10;
    for (idx_t k = 3; k <= n; ++k)
    {
        if (((k - 3) % 7) < 3)
            result += (seq_t)4;
        else
            result += (seq_t)5;
    }
    return result;
}
