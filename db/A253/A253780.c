/*
 * A253780  Cubes c^3 that are equal to the sum of m^3 consecutive cubes
 *          starting at b^3 with b (A253778) for m^3 not divisible by 3
 *          (A118719).
 * https://oeis.org/A253780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( m(m^2-1)(m^2+2)/6 )^3 where m = nth positive integer
 *            not divisible by 3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 18, u128 n <= 354,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = nth integer not divisible by 3 */
    idx_t m = n + (n - 1) / 2;   /* 1→1, 2→2, 3→4, 4→5, … */
    
    seq_t mm = (seq_t)m;
    seq_t m2 = mm * mm;           /* m^2 */
    seq_t numerator = mm * (m2 - 1) * (m2 + 2);  /* m*(m^2-1)*(m^2+2) */
    
    seq_t c = numerator / 6;      /* c is integer */
    seq_t result = c * c * c;     /* c^3 */
    
    return result;
}
