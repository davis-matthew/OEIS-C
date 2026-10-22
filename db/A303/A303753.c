/*
 * A303753  Ordinal transform of cototient (A051953).
 * https://oeis.org/A303753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 ≤ k ≤ n, such that k - φ(k) = n - φ(n)
 * Range:     u8 n <= 1618, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In particular, for prime p, a(p) = index of p among the primes. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate and compute Euler's totient φ up to n. */
    idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!phi)
        return (seq_t)0;          /* OOM guard – unlikely for n≤10000. */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;
    if (n >= 1)
        phi[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {            /* i is prime */
            phi[i] = i - 1;
            for (idx_t j = i << 1; j <= n; j += i) {
                phi[j] = (phi[j] / i) * (i - 1);
            }
        }
    }

    /* Allocate count array for cototients (value ≤ n-1). */
    seq_t *cnt = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cnt) {
        free(phi);
        return (seq_t)0;
    }
    memset(cnt, 0, (n + 1) * sizeof(seq_t));

    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t f = k - phi[k];               /* cototient of k (≤ n-1) */
        cnt[f]++;                            /* include k itself */
        if (k == n)
            result = cnt[f];
    }

    free(cnt);
    free(phi);
    return result;
}
