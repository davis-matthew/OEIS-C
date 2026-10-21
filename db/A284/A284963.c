/*
 * A284963  Numbers with digits 3 and 8 only.
 * https://oeis.org/A284963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mapping: interpret n in binary to produce a number with
   digits 3 and 8 only.  For each index n (1‑based) we
   find the minimal length l such that 2^l ≥ n+1,
   then form a decimal number of length l where
   a binary 0 maps to digit 3 and a binary 1 maps to 8. */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* undefined; safe return */

    /* Length of the decimal representation */
    idx_t l = floor_log2_uint64(n + 1);

    /* Index of the first number with this length */
    idx_t start = ((idx_t)1 << l) - 1;
    idx_t idx   = n - start;   /* 0‑based offset within the block */

    seq_t num = 0;
    for (idx_t bit = l; bit > 0; )
    {
        --bit;
        seq_t digit = (idx >> bit) & 1 ? 8 : 3;
        num = num * (seq_t)10 + digit;
    }
    return num;
}
