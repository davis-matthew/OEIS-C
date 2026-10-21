/*
 * A281356  G.f.: 1 + Sum_{n>=1} x^(3*n-2) / Product_{k=1..n} (1-x^k).
 * https://oeis.org/A281356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281356
 * Formula: a(n) = sum_{k=1..floor((n+2)/3)} p(n-3k+2, k)
 * where p(m, k) is the number of partitions of m into parts ≤ k.
 * This is equivalent to extracting coefficients from
 * G(x) = 1 + Σ_{k≥1} x^{3k-2} / Π_{j=1..k} (1 – x^j).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    /* maximum n for which partitions are needed */
    idx_t Nmax = (n + 2) / 3;   /* floor((n+2)/3) */

    /* allocate array for dynamic programming of partitions */
    size_t size = (size_t)n + 1;
    seq_t *ways = (seq_t *)malloc(size * sizeof(seq_t));
    if (!ways) return 0;          /* memory allocation failure */

    /* initialize partitions with 0 parts:
       p(0,0) = 1, all others 0 */
    for (size_t i = 0; i < size; ++i) ways[i] = 0;
    ways[0] = 1;

    seq_t sum = 0;

    for (idx_t part = 1; part <= Nmax; ++part) {
        /* update partitions to include parts of size 'part' unlimited times */
        for (idx_t m = part; m <= n; ++m) {
            ways[m] += ways[m - part];
        }

        /* add contribution for current 'part' */
        if (n >= 3 * part - 2) {
            idx_t k_req = n - 3 * part + 2;
            /* k_req is guaranteed <= n and >= 0 here */
            sum += ways[k_req];
        }
    }

    free(ways);
    return sum;
}
