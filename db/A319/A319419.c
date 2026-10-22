/*
 * A319419  In binary expansion of n, delete one symbol from each run. Set
 *          a(n)=-1 if the result is the empty string.
 * https://oeis.org/A319419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, hear
 * Formula:   not stated
 * Range:     i8 n <= 510, i16 n <= 9944, i32 n <= 9944, i64 n <= 9944,
 *            i128 n <= 9944, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A319419:
   In binary representation of n, delete one symbol from each run.
   The resulting binary string (without leading zeros) is interpreted as
   a number; if the string is empty then return -1. */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* 0 has one run of zero bits, becomes empty */
        return -1;

    int msb = (int)floor_log2_uint64(n);  /* position of most‑significant 1 */
    seq_t result = 0;
    int has_bit = 0;
    int i = msb;

    while (i >= 0) {
        int bit   = (int)((n >> i) & 1);
        int start = i;

        /* Move to the end of the current run of equal bits */
        while (i > 0 && ((n >> (i-1)) & 1) == bit)
            i--;

        int run_len = start - i + 1;   /* length of this run   */
        int new_len = run_len - 1;     /* delete one symbol   */

        if (new_len > 0) {
            for (int j = 0; j < new_len; ++j)
                result = (result << 1) | bit;
            has_bit = 1;
        }
        i--;   /* move past this run to the next lower bit   */
    }

    return has_bit ? result : -1;
}
