/*
 * A266150  Take the binary representation of n, increase each run of 0's by
 *          one 0 if the length of run is odd, otherwise, if length of run is
 *          even, remove one 0. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A266150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the binary string obtained from bin(n)
 *            by replacing each run of zeros: if the run length is odd, extend
 *            it by one zero; if the run length is even, shorten it by one
 *            zero
 * Range:     u8 n <= 41, u16 n <= 2217, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t bitlen(idx_t x)
{
    idx_t len = 0;
    do {
        ++len;
        x >>= 1;
    } while (x);
    return len;
}

seq_t a(idx_t n)
{
    /* length of the binary representation of n
       (for n = 0 the length is defined as 1) */
    idx_t len = bitlen(n);

    seq_t result = 0;        /* output accumulated value */
    idx_t zero_run = 0;      /* current run length of zeros */

    /* scan bits from most significant to least significant */
    for (idx_t i = len; i > 0; --i) {
        idx_t bitPos = i - 1;
        int bit = (n >> bitPos) & 1;   /* 0 or 1 */

        if (bit == 0) {
            ++zero_run;
        } else { /* bit == 1 */
            if (zero_run > 0) {
                idx_t newLen = (zero_run & 1) ? (zero_run + 1) : (zero_run - 1);
                for (idx_t k = 0; k < newLen; ++k) {
                    result <<= 1;          /* emit a zero bit */
                }
                zero_run = 0;
            }
            result = (result << 1) | 1;   /* emit the 1 bit */
        }
    }

    /* handle trailing zero run (if any) */
    if (zero_run > 0) {
        idx_t newLen = (zero_run & 1) ? (zero_run + 1) : (zero_run - 1);
        for (idx_t k = 0; k < newLen; ++k) {
            result <<= 1;              /* emit a zero bit */
        }
    }

    return result;
}
