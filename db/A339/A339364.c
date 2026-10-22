/*
 * A339364  Number of partitions of n into an even number of squares.
 * https://oeis.org/A339364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into an even number of squares.
 * Computation: dynamic programming that keeps track of parity.
 *   even[i] = number of partitions of i with an even number of parts.
 *   odd[i]  = number of partitions of i with an odd  number of parts.
 * For each square s = k^2 we update:
 *   even[i] += odd[i - s]   (adding a square turns an odd‑part partition even)
 *   odd[i]  += even[i - s]  (adding a square turns an even‑part partition odd)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle offset n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* allocate DP arrays of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *even = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!even || !odd) abort();

    even[0] = (seq_t)1;   /* empty partition has 0 parts (even) */

    /* unlimited coin change over squares, tracking parity */
    for (idx_t k = (idx_t)1; ; ++k) {
        idx_t s = k * k;
        if (s > n) break;
        for (idx_t i = s; i <= n; ++i) {
            size_t ii = (size_t)i;
            size_t is = (size_t)(i - s);
            even[ii] += odd[is];
            odd[ii]  += even[is];
        }
    }

    seq_t result = even[(size_t)n];
    free(even);
    free(odd);
    return result;
}
