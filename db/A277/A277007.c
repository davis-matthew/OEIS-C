/*
 * A277007  Number of maximal runs of 1-bits (in binary expansion of n) such
 *          that the length of run > 1 + the total number of zeros anywhere
 *          right of that run.
 * https://oeis.org/A277007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of maximal runs of 1‑bits in the binary
 *            representation of n for which run_length > 1 + the number of
 *            0‑bits to the right of that run
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, if desired */

seq_t a(idx_t n)
{
    /* Count zeros seen in lower bits, current run length of 1‑bits,
       and total number of qualifying runs. */
    idx_t zeros_seen = 0;   /* number of 0‑bits in bits processed so far */
    idx_t run_len    = 0;   /* length of the current run of 1‑bits */
    seq_t count      = 0;   /* result */

    idx_t temp = n;
    while (temp != 0)
    {
        if (temp & 1)
        {
            /* still in a run of 1‑bits */
            run_len++;
        }
        else
        {
            /* zero ends any run that is currently being counted */
            if (run_len > 0)
            {
                if (run_len > 1 + zeros_seen) count++;
                run_len = 0;
            }
            zeros_seen++;               /* this zero belongs to bits right of any future run */
        }
        temp >>= 1;  /* move to the next less‑significant bit */
    }

    /* Flush the last run if the number ends with 1‑bits */
    if (run_len > 0)
    {
        if (run_len > 1 + zeros_seen) count++;
    }

    return count;
}
