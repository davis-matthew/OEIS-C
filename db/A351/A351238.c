/*
 * A351238  Numbers M such that 87 * M = 1M1, where 1M1 denotes the
 *          concatenation of 1, M and 1.
 * https://oeis.org/A351238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(6*n-3)+1)/77 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Build the decimal representation: (n‑1) copies of "129870" followed by "13" */
    if (n == 0) return 0;          /* undefined for n<1; return 0 as neutral value */
    size_t reps = (size_t)(n - 1);  /* number of repeated blocks */
    size_t len  = 6 * reps + 2;     /* total number of decimal digits */

    char *buf = (char *)malloc(len + 1); /* +1 for terminating null */
    if (!buf) return 0; /* allocation failed */

    const char pattern[7] = "129870";
    for (size_t i = 0; i < reps; ++i)
        memcpy(buf + i * 6, pattern, 6);

    /* append the final "13" */
    buf[len - 2] = '1';
    buf[len - 1] = '3';
    buf[len] = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
