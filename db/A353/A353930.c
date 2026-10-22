/*
 * A353930  Smallest number whose binary expansion has n distinct run-sums.
 * https://oeis.org/A353930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A353930 – smallest number whose binary expansion has n distinct run‑sums.
    Construction: for n = 1 return 1.
    For n ≥ 2 build the binary string
          1^1 0 1^2 0 … 1^(n‑1)
    (1^k = k consecutive ones, 0 = a single zero).
    Then return its numeric value.  This yields the sequence given in the OEIS. */

#include <stdint.h>   /* for fixed‑width types, if needed */

seq_t a(idx_t n)
{
    /* Handle the minimal case explicitly. */
    if (n == 1)
        return (seq_t)1;   /* binary "1" has run‑sum {1} */

    seq_t x = 0;
    for (idx_t i = 1; i < n; ++i) {
        /* Append a run of i ones:  (1<<i) - 1  shifted into place */
        seq_t ones = ((seq_t)1 << (unsigned long)i) - 1;
        x = (x << (unsigned long)i) | ones;

        /* Insert a single zero after the run, except when it is the
           last run and n > 2.  For n == 2 the trailing zero is needed. */
        if (i < n - 1 || n == 2)
            x <<= 1;
    }
    return x;
}
