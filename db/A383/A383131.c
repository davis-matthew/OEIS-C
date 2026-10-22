/*
 * A383131  a(n) is the number of iterations that n requires to reach 1 under
 *          the map x -> -x/2 if x is even, 3x + 1 if x is odd; a(n) = -1 if 1
 *          is never reached.
 * https://oeis.org/A383131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations needed for n to reach 1 using the
 *            map: x -> -x/2 if x is even, 3x+1 if x is odd. Returns -1 (i.e.
 *            (seq_t)-1) if 1 is never reached
 * Range:     u8 n <= 2378, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)-1;          /* out of range: treat as missing */
    long long x = (long long)n;           /* use signed type for intermediate values */
    seq_t steps = 0;
    /* Iterate until reaching 1 */
    while (x != 1) {
        if ((x & 1LL) == 0) {            /* even case */
            x = -x / 2;
        } else {                         /* odd case */
            x = 3 * x + 1;
        }
        steps++;                         /* count this step */
    }
    return steps;
}
