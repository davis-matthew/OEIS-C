/*
 * A335835  Sort the run lengths in binary expansion of n in descending order
 *          (with multiplicities).
 * https://oeis.org/A335835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) is constructed by sorting the run lengths of the binary
 *            representation of n in descending order, then reassembling a
 *            binary number that starts with 1 and alternates runs of 1s and
 *            0s
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for descending order */
static int cmp_desc(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x < y) - (x > y);   /* returns +1 if x<y, -1 if x>y */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Determine the number of bits in n */
    int bits = 0;
    idx_t temp = n;
    while (temp) {
        bits++;
        temp >>= 1;
    }

    /* Extract run lengths into an array */
    int runs[65];
    int rc = 0;                 /* number of runs */

    int current_bit = (int)((n >> (bits - 1)) & 1);
    int count = 1;

    for (int pos = bits - 2; pos >= 0; --pos) {
        int bit = (int)((n >> pos) & 1);
        if (bit == current_bit) {
            count++;
        } else {
            runs[rc++] = count;
            current_bit = bit;
            count = 1;
        }
    }
    runs[rc++] = count;         /* last run */

    /* Sort the run lengths in descending order */
    qsort(runs, rc, sizeof(int), cmp_desc);

    /* Reconstruct the binary number from sorted run lengths */
    seq_t res = 0;
    for (int i = 0; i < rc; ++i) {
        int len = runs[i];
        res <<= len;                    /* shift to make room for the run */

        /* If it's a run of 1s (odd-indexed run, starting with 1) */
        if ((i & 1) == 0) {
            seq_t ones = (((seq_t)1 << len) - 1);   /* len ones */
            res |= ones;
        }
    }
    return res;
}
