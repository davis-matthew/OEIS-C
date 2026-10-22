/*
 * A329814  The smallest base b where the sum of the digits for the number n
 *          in the base b is the smallest, with 1 < b < n and a(1) = a(2) = 1.
 * https://oeis.org/A329814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the smallest base b with 1 < b < n whose representation
 *            of n has the minimal digit sum. a(1) = a(2) = 1
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the defined initial values */
    if (n <= 2) {
        return (seq_t)1;
    }

    /* Compute digit sum for base 2 first */
    seq_t bestBase = (seq_t)2;
    seq_t bestSum = 0;
    seq_t tmp = (seq_t)n;
    while (tmp) {
        bestSum += tmp & 1;   /* tmp % 2 == tmp & 1 (fast for power of 2) */
        tmp >>= 1;            /* tmp /= 2   */
    }

    /* Iterate over remaining bases */
    for (idx_t b = 3; b < n; ++b) {
        seq_t base = (seq_t)b;
        seq_t t = (seq_t)n;
        seq_t s = 0;
        while (t) {
            s += t % base;
            t /= base;
        }
        if (s < bestSum) {
            bestSum = s;
            bestBase = base;
        }
    }

    return bestBase;
}
