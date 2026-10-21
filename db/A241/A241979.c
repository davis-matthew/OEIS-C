/*
 * A241979  (0,1) sequence such that lengths of three consecutive runs are
 *          always distinct.
 * https://oeis.org/A241979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) has period 12 with a(n+6)=1-a(n). The base pattern of
 *            length 6 is {0,1,1,0,0,0}. For any n compute n/6 and n%6; if n/6
 *            is odd the value is flipped
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for 0≤k<6 */
    static const seq_t base[6] = {0,1,1,0,0,0};

    seq_t val = base[n % 6];          /* value in the first 6 positions */
    val ^= ( (n / 6) & 1 );           /* flip if necessary */
    return val;
}
