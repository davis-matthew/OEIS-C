/*
 * A360274  a(n) = Sum_{k=0..floor(n/3)} Catalan(n-3*k).
 * https://oeis.org/A360274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} Catalan(n-3k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for Catalan numbers up to n */
    seq_t *cat = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cat) return 0;          /* allocation failed */

    cat[0] = (seq_t)1;           /* C_0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        /* Compute C_{i+1} = C_i * 2*(2*i+1)/(i+2) with 128‑bit safety */
        unsigned __int128 prod = (unsigned __int128)cat[i]
                               * (unsigned __int128)(2 * (2*i + 1));
        cat[i+1] = (seq_t)(prod / (unsigned __int128)(i + 2));
    }

    /* Sum Catalan(n-3k) for k = 0,1,...,floor(n/3) */
    seq_t sum = (seq_t)0;
    for (idx_t i = n; ; i -= 3) {
        sum += cat[i];
        if (i < 3) break;
    }

    free(cat);
    return sum;
}
