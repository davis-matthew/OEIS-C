/*
 * A297779  Number of distinct runs in base-11 digits of n.
 * https://oeis.org/A297779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct digits in the base‑11 representation
 *            of n. Example: 11! = 39916800 decimal = 2 0 5 9 4 0 10 0
 *            (base‑11). There are 6 distinct digits {0,2,5,9,4,10}, so a(11!)
 *            = 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of range, returns 0 */
    unsigned int mask = 0;                  /* bitmask for digits 0..10 */
    idx_t x = n;
    while (x > 0) {
        unsigned int d = x % 11;            /* current base‑11 digit */
        mask |= 1u << d;                    /* record digit */
        x /= 11;
    }
    seq_t cnt = 0;
    while (mask) {                          /* count set bits */
        cnt++;
        mask &= mask - 1;
    }
    return cnt;
}
