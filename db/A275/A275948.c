/*
 * A275948  Number of nonzero digits that occur only once in factorial base
 *          representation of n: a(n) = A056169(A275735(n)).
 * https://oeis.org/A275948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonzero digits that occur exactly once in the
 *            factorial base representation of n
 * Range:     u8 n <= 9924, u16 n <= 9924, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Frequency table for digits 0..20 (max digit value for n <= 2^64-1 is 20) */
    int freq[21] = {0};

    idx_t tmp = n;
    int i = 1;                     /* position index (1-based) */
    while (tmp > 0) {
        int d = (int)(tmp % (i + 1));   /* digit for position i */
        freq[d]++;                      /* count occurrence of digit d */
        tmp /= (i + 1);                 /* move to next position */
        i++;
    }

    seq_t count = 0;
    for (int d = 1; d <= 20; d++) {      /* ignore zero digits */
        if (freq[d] == 1) {
            count++;
        }
    }
    return count;
}
