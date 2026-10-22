/*
 * A380759  Number of coincident digits occurring in expression of integers in
 *          both base 2 and base 10.
 * https://oeis.org/A380759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ_{d=0}^{9} min(count of digit d in decimal
 *            representation of n, count of digit d in binary representation
 *            of n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Count decimal digits of n */
    unsigned char dec[10] = {0};
    idx_t tmp = n;
    if (tmp == 0) {
        dec[0] = 1;
    } else {
        while (tmp) {
            unsigned d = tmp % 10;
            dec[d]++;
            tmp /= 10;
        }
    }

    /* Count bits of n in base 2 */
    int bits  = 0;
    int ones  = 0;
    tmp = n;
    if (tmp == 0) {
        bits = 1;   /* single zero bit */
        ones = 0;
    } else {
        while (tmp) {
            bits++;
            if (tmp & 1) ones++;
            tmp >>= 1;
        }
    }
    int zeros = bits - ones;

    /* Add coincidences for digits 0 and 1 only (others cannot match) */
    res += (dec[0] < zeros) ? dec[0] : zeros;   /* digit 0 */
    res += (dec[1] < ones)   ? dec[1] : ones;   /* digit 1 */

    return res;
}
