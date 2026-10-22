/*
 * A334297  Length of the Lyndon factorization of the reversed n-th
 *          composition in standard order.
 * https://oeis.org/A334297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the Lyndon factorization of the reversed n-th
 *            composition in standard order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for NULL, if needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Step 1: collect positions of 1‑bits in the reversed binary
       expansion of n (the set bits of n). */
    idx_t pos[65];
    int poscnt = 0;
    idx_t tmp = n;
    idx_t bitpos = 1;
    while (tmp) {
        if (tmp & 1)
            pos[poscnt++] = bitpos;
        tmp >>= 1;
        bitpos++;
    }

    /* Step 2: compute the first differences – the composition. */
    idx_t comp[65];
    int m = poscnt;               /* length of the composition */
    idx_t prev = 0;
    for (int i = 0; i < m; ++i) {
        comp[i] = pos[i] - prev;
        prev = pos[i];
    }

    /* Step 3: Duval's algorithm to count Lyndon factors of `comp`. */
    seq_t count = 0;
    int i = 0;
    while (i < m) {
        int j = i + 1;
        int k = i;
        while (j < m && comp[k] <= comp[j]) {
            if (comp[k] < comp[j])
                k = i;
            else
                k++;
            j++;
        }
        int l = j - k;
        while (i <= k) {
            count++;
            i += l;
        }
    }

    return count;
}
