/*
 * A384184  Order of the permutation of {0,...,n-1} formed by successively
 *          swapping elements at i and 2*i mod n, for i = 0,...,n-1.
 * https://oeis.org/A384184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of the permutation obtained by swapping elements at
 *            indices i and 2*i (mod n) for i = 0 .. n-1
 * Range:     u8 n <= 68, u16 n <= 134, u32 n <= 558, u64 n <= 2006,
 *            u128 n <= 9846, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safeguard – offset starts at 1 */

    /* Allocate arrays for the permutation and visited flags. */
    idx_t *perm = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!perm)
        return (seq_t)0;   /* allocation failure (caller should check) */

    unsigned char *seen = (unsigned char *)calloc((size_t)n, 1);
    if (!seen) {
        free(perm);
        return (seq_t)0;
    }

    /* Build the permutation by successive swaps. */
    for (idx_t i = 0; i < n; ++i)
        perm[i] = i;
    for (idx_t i = 0; i < n; ++i) {
        idx_t j = ((unsigned long long)i * 2ULL) % n;
        idx_t tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
    }

    /* Compute the lcm of the cycle lengths – the order of the permutation. */
    seq_t lcm_val = 1;
    for (idx_t i = 0; i < n; ++i) {
        if (!seen[i]) {
            idx_t cur = i;
            idx_t len = 0;
            while (!seen[cur]) {
                seen[cur] = 1;
                cur = perm[cur];
                ++len;
            }
            seq_t g = gcd(lcm_val, (seq_t)len);
            lcm_val = (lcm_val / g) * (seq_t)len;
        }
    }

    free(perm);
    free(seen);
    return lcm_val;
}
