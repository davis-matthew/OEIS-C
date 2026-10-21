/*
 * A272664  (001)(001)(001)(10)*.
 * https://oeis.org/A272664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n < 9 ? first[n] : (n & 1))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* first[0..8] = {0,0,1,0,0,1,0,0,1} match the initial terms,  
   after n≥9 the sequence is simply 0 for even n and 1 for odd n. */

seq_t a(idx_t n)
{
    static const seq_t first[9] = {0,0,1,0,0,1,0,0,1};
    if (n < 9)
        return first[n];
    return (seq_t)(n & 1);
}
