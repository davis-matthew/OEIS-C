/*
 * A345918  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum > 0.
 * https://oeis.org/A345918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the reverse-alternating sum
 *            of the k-th composition (graded reverse-lexicographic order,
 *            A066099) is > 0. The function reaches k by brute force
 *            enumeration of compositions, computing the sum on the fly
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;          /* how many qualifying k's found */
    idx_t k = 1;              /* current candidate k */
    while (count < n) {
        idx_t k_copy = k;
        idx_t pos = 1;            /* position of current bit when scanning LSB->MSB */
        idx_t prev = 0;           /* previous position of a 1-bit (start at 0) */
        __int128 sum = 0;         /* reverse-alternating sum, can be negative */
        int sign = 1;             /* +1 for first part, then alternating */

        /* Scan bits of k from least significant to most significant */
        while (k_copy) {
            if (k_copy & 1U) {    /* if this bit is a 1, produce a part */
                idx_t part = pos - prev;   /* difference between positions */
                sum += (__int128)sign * part;
                sign = -sign;               /* toggle sign for next part */
                prev = pos;                 /* update previous 1-bit position */
            }
            pos++;                     /* move to next bit position */
            k_copy >>= 1;              /* shift to next bit */
        }

        if (sum > 0) {                /* positive reverse-alternating sum */
            count++;
            if (count == n)
                return (seq_t)k;       /* found the desired k */
        }
        k++;                          /* try next integer */
    }
    return 0;                         /* should never reach here for n>=1 */
}
