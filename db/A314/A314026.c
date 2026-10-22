/*
 * A314026  Coordination sequence Gal.5.130.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k+1) = 5*(2k+1); a(2k) = 10*k + f(k) where f(0)=f(1)=1, and
 *            for k≥2, f(k) follows the period 0, –1, 0, 1 (i.e. f(k) = 0 if
 *            (k‑2)%4==0 or 2; f(k) = –1 if (k‑2)%4==1; f(k) = 1 if
 *            (k‑2)%4==3)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1ULL) {                   /* odd n : a(n) = 5*n */
        return (seq_t)5 * (seq_t)n;
    }

    idx_t k = n >> 1;                 /* even n = 2k */
    if (k <= 1) {                     /* special cases k=0,1 -> f(k)=1 */
        seq_t val = (seq_t)10 * (seq_t)k;
        val += (seq_t)1;              /* add +1 adjustment */
        return val;
    }

    int m = (int)((k - 2) & 3ULL);    /* (k‑2) mod 4 */
    int f = 0;
    if (m == 1)      f = -1;
    else if (m == 3) f =  1;

    seq_t val = (seq_t)10 * (seq_t)k; /* base 10*k = 5*n */
    if (f > 0)   val += (seq_t)f;
    else if (f < 0) val -= (seq_t)(-f);
    return val;
}
