/*
 * A296102  Number of total dominating sets in the n-prism graph.
 * https://oeis.org/A296102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-3) - 2*a(n-4) + 10*a(n-5) + 4*a(n-6) -
 *            a(n-8) - a(n-9) Initial values: a(1)=3, a(2)=9, a(3)=39,
 *            a(4)=121, a(5)=443, a(6)=1521, a(7)=5071, a(8)=17161, a(9)=58035
 *            Compute by an explicit linear recurrence; no special‑case table
 *            is needed. The routine works for any index order...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but harmless */

    /* Allocate temporary array of size n+1 (index 0 unused). */
    seq_t *ab = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!ab) abort();

    /* Fill known initial values. */
    if (n >= 1) ab[1] = (seq_t)3;
    if (n >= 2) ab[2] = (seq_t)9;
    if (n >= 3) ab[3] = (seq_t)39;
    if (n >= 4) ab[4] = (seq_t)121;
    if (n >= 5) ab[5] = (seq_t)443;
    if (n >= 6) ab[6] = (seq_t)1521;
    if (n >= 7) ab[7] = (seq_t)5071;
    if (n >= 8) ab[8] = (seq_t)17161;
    if (n >= 9) ab[9] = (seq_t)58035;

    /* Compute the remaining values by the recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        ab[i] = (seq_t)3 * ab[i - 1] +
                (seq_t)4 * ab[i - 3] -
                (seq_t)2 * ab[i - 4] +
                (seq_t)10 * ab[i - 5] +
                (seq_t)4 * ab[i - 6] -
                ab[i - 8] -
                ab[i - 9];
    }

    seq_t result = ab[n];
    free(ab);
    return result;
}
