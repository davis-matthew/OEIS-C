/*
 * A396385  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=3/2.
 * https://oeis.org/A396385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fⁿ(n), where f(x) = ⌊3x/2⌋ is applied n times to the
 *            starting value n
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = ⌊3/2⌋ = 1
   a(2) = ⌊3⌋ = 4
   a(3) = ⌊6⌋ = 9
   ... */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
    {
        /* val ← ⌊3·val/2⌋  = val + ⌊val/2⌋  (avoids overflow for values
           that fit in seq_t). */
        val = val + val / (seq_t)2;
    }
    return val;
}
