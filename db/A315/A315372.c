/*
 * A315372  Coordination sequence Gal.6.567.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315372
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

/* Coordination sequence A315372 (Gal.6.567.6)
 *
 * The sequence satisfies a(0)=1 and for n≥1:
 *   a(4k)   = 5·n                      (k>0)
 *   a(4k+1) = 5·n + (k even ? 1 : -2k)
 *   a(4k+2) = 5·n + (k even ? 1-2k : -(2k+3))
 *   a(4k+3) = 5·n + (k even ? -(2k+2) : -1)
 *
 * Equivalently:
 *   if (n%4==0) a = 5*n;
 *   else if (n%4==1) a = 5*n + ((k&1)? -2*k : 1);
 *   else if (n%4==2) a = 5*n + ((k&1)? -(2*k+3) : 1-2*k);
 *   else             a = 5*n + ((k&1)? -1 : -(2*k+2));
 *
 * The function handles unsigned indices of arbitrary size and
 * returns the exact value in type seq_t (unsigned integer or arbitrary-precision).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    seq_t res = 5 * (seq_t)n;     /* 5·n for all n≥1 */

    idx_t k = n / 4;              /* floor(n/4) */
    idx_t r = n % 4;              /* n mod 4 */

    if (r == 1) {
        if ((k & 1) == 0)          /* k even */
            res += 1;
        else                       /* k odd  */
            res -= 2 * (seq_t)k;
    } else if (r == 2) {
        if ((k & 1) == 0)          /* k even */
            res -= (2 * (seq_t)k - 1);    /* 1 - 2k */
        else                       /* k odd  */
            res -= (2 * (seq_t)k + 3);
    } else if (r == 3) {
        if ((k & 1) == 0)          /* k even */
            res -= (2 * (seq_t)k + 2);
        else                       /* k odd  */
            res -= 1;
    }

    return res;
}
