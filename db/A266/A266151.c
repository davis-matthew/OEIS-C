/*
 * A266151  Take the binary representation of n, increase each run of 1's by
 *          one 1 if the length of run is odd, otherwise, if length of run is
 *          even, remove one 1. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A266151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = transform described in the problem statement
 * Range:     u8 n <= 36, u16 n <= 1364, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return 0;

    /* Extract bits of n into array: bits[0] = LSB */
    unsigned char bits[256];
    unsigned int len = 0;
    seq_t tmp = (seq_t)n;
    while (tmp > 0) {
        bits[len++] = (unsigned char)(tmp & 1);
        tmp >>= 1;
    }

    /* Process bits from MSB to LSB, building transformed bit string */
    unsigned char out[512];
    unsigned int outlen = 0;
    unsigned int run = 0; /* length of current run of 1's */

    for (int i = (int)len - 1; i >= 0; --i) {
        unsigned char bit = bits[i];
        if (bit == 1) {
            ++run;               /* continue the run */
        } else { /* bit == 0 */
            if (run > 0) {
                unsigned int newlen = (run & 1) ? run + 1 : run - 1;
                for (unsigned int j = 0; j < newlen; ++j)
                    out[outlen++] = 1; /* add the transformed run */
                run = 0;
            }
            out[outlen++] = 0;   /* keep this zero bit */
        }
    }

    /* Handle trailing run of 1's, if any */
    if (run > 0) {
        unsigned int newlen = (run & 1) ? run + 1 : run - 1;
        for (unsigned int j = 0; j < newlen; ++j)
            out[outlen++] = 1;
    }

    /* Convert resulting bits to an integer */
    seq_t result = 0;
    for (unsigned int i = 0; i < outlen; ++i)
        result = (result << 1) | (seq_t)out[i];

    return result;
}
