/*
 * A300076  A sequence based on the period 6 sequence A300075.
 * https://oeis.org/A300076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A300075(n) + 3*floor(n/6), where A300075(n) =
 *            {0,1,1,2,2,2} repeating every 6
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t block = n / 6;          /* floor(n/6) */
    seq_t add   = (seq_t)3 * (seq_t)block;

    idx_t m = n % 6;
    seq_t pat;
    switch (m) {
        case 0: pat = (seq_t)0; break;
        case 1: pat = (seq_t)1; break;
        case 2: pat = (seq_t)1; break;
        case 3: pat = (seq_t)2; break;
        case 4: pat = (seq_t)2; break;
        default: pat = (seq_t)2; break;   /* case 5 */
    }
    return add + pat;
}
