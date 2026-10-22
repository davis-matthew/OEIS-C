/*
 * A370470  In the binary expansion of n, change the i-th run length to the
 *          difference of the i-th and (i-1)-th run lengths. The first run
 *          length remains as is.
 * https://oeis.org/A370470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the binary number obtained by starting with the binary
 *            expansion of n, replacing each run length after the first with
 *            the absolute difference between that run length and the previous
 *            run length, then concatenating the runs (starting with a run of
 *            1's). The resulting binary string is interp...
 * Range:     u8 n <= 61, u16 n <= 1021, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe guard */

    /* 1. extract run lengths of n's binary representation */
    idx_t temp = n;
    size_t msb = 0;
    while ((temp >> (msb + 1)) != 0) msb++;          /* position of highest bit */

    size_t runlens[70];     /* enough for any 64‑bit number */
    size_t runcnt = 0;

    int current_bit = (int)((n >> msb) & 1);         /* should be 1 */
    size_t current_len = 1;                          /* we have already counted one bit */

    for (long long i = (long long)msb - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1);
        if (bit == current_bit) {
            ++current_len;
        } else {
            runlens[runcnt++] = current_len;
            current_bit = bit;
            current_len = 1;
        }
    }
    runlens[runcnt++] = current_len;                 /* last run */

    /* 2. compute modified run lengths and build the result */
    seq_t val = 0;
    int curbit = 1;   /* first run is ones */

    for (size_t i = 0; i < runcnt; ++i) {
        size_t mod_len;
        if (i == 0) {
            mod_len = runlens[0];
        } else {
            mod_len = (runlens[i] > runlens[i-1])
                      ? runlens[i] - runlens[i-1]
                      : runlens[i-1] - runlens[i];
        }

        /* Append 'mod_len' bits of curbit to val */
        for (size_t j = 0; j < mod_len; ++j) {
            val = (val << 1) | (seq_t)curbit;
        }

        curbit ^= 1;     /* flip to the other run type */
    }

    return val;
}
