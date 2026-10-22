/*
 * A312736  Coordination sequence Gal.5.268.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312736
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

/* 
   Formula for the sequence A312736 (coordination sequence Gal.5.268.1):
   a(0) = 1
   For n > 0:
        let k = n / 6   and   r = n % 6
        d(n) = 
           r == 0   : -2
           r == 1   : (k == 0 ? -1 : -5*k + 1)
           r == 2   : -2 + k
           r == 3   : -4*k
           r == 4   : -1 + k
           r == 5   : -2 - 5*k
        a(n) = 5*n + d(n)
   This piecewise linear formula reproduces all values of the sequence.
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    idx_t k = n / 6;                    /* integer division */
    int r  = (int)(n % 6);              /* remainder 0..5 */
    int d;

    switch (r) {
    case 0: d = -2; break;
    case 1: d = (k == 0) ? -1 : -5 * (int)k + 1; break;
    case 2: d = -2 + (int)k; break;
    case 3: d = -4 * (int)k; break;
    case 4: d = -1 + (int)k; break;
    case 5: d = -2 - 5 * (int)k; break;
    default: d = 0; /* never reached */
    }

    seq_t res = ((seq_t)5) * ((seq_t)n);   /* 5 * n  */
    if (d >= 0)
        res += (seq_t)d;
    else
        res -= (seq_t)(-d);

    return res;
}
