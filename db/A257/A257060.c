/*
 * A257060  Number of length n 1..(6+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) + a(n-3) with initial values a(1)=4,
 *            a(2)=18, a(3)=81
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, harmless */

    static seq_t *buf = NULL;
    static idx_t len = 0;                /* highest index that has been computed */

    /* Ensure buffer is allocated at least up to index n. */
    idx_t required = n + 1;              /* buf[0] is unused, we need indices 1..n */

    if (buf == NULL) {
        /* Allocate space and initialise the first three terms. */
        /* At least 4 slots are needed to store indices 1..3. */
        if (required < 4) required = 4;
        buf = (seq_t*)malloc(sizeof(seq_t) * (size_t)required);
        if (!buf) return (seq_t)0;      /* allocation failed */
        buf[1] = (seq_t)4;
        buf[2] = (seq_t)18;
        buf[3] = (seq_t)81;
        len   = 3;
    } else if (required > len) {
        seq_t *tmp = (seq_t*)realloc(buf, sizeof(seq_t) * (size_t)required);
        if (!tmp) return (seq_t)0;      /* reallocation failed */
        buf = tmp;
    }

    /* Compute any missing values up to n using the recurrence. */
    if (len < n) {
        for (idx_t i = len + 1; i <= n; ++i) {
            buf[i] = (seq_t)4 * buf[i-1] + (seq_t)2 * buf[i-2] + buf[i-3];
        }
        len = n;
    }

    return buf[n];
}
