/*
 * A246552  2-adic valuation of the number of involutions of n (A000085).
 * https://oeis.org/A246552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2*floor(n/4) - floor((n+3)/4) Simplified: let q = n
 *            >> 2; r = n & 3; a(n) = q + (r==2?1 : r==3?2 : 0)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)(n >> 2);          /* q = floor(n/4) */
    switch (n & 3)                      /* r = n mod 4 */
    {
        case 2: return q + 1;           /* r==2 → +1 */
        case 3: return q + 2;           /* r==3 → +2 */
        default: return q;              /* r==0 or 1 → +0 */
    }
}
