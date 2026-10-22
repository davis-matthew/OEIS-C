/*
 * A313333  Coordination sequence Gal.6.452.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313333
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
 * Coordination sequence A313333 (Gal.6.452.1)
 *
 * The sequence follows the linear law a(n) = 5*n plus a small periodic
 * adjustment that depends on n modulo 5.  The adjustment is
 *
 *   if n == 0     :  +1   (special initial value)
 *   if n mod 5 == 0 :  +0
 *   if n mod 5 == 1 :  -(n/5 + 1)
 *   if n mod 5 == 2 :  -1
 *   if n mod 5 == 3 :  +1
 *   if n mod 5 == 4 :  -(n/5)
 *
 *  i.e. for n ≥ 1:
 *
 *   a(n) = 5*n + 
 *          { 0                , n % 5 == 0
 *            -(n/5 + 1)       , n % 5 == 1
 *            -1                , n % 5 == 2
 *            +1                , n % 5 == 3
 *            -(n/5)           , n % 5 == 4 }
 *
 *  The special value a(0) = 1 is handled explicitly.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t k = n / 5;          /* floor(n/5)              */
    int    r = n % 5;         /* 0 .. 4, safe to cast   */
    seq_t val = 5 * (seq_t)n; /* base 5*n                */

    switch (r) {
        case 1:
            val -= (seq_t)(k + 1);
            break;
        case 2:
            val -= 1;
            break;
        case 3:
            val += 1;
            break;
        case 4:
            val -= (seq_t)k;
            break;
        /* case 0: no adjustment */
        default:
            break;
    }
    return val;
}
