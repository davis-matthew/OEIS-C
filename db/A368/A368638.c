/*
 * A368638  a(n) is the number of triangular partitions whose Young diagram
 *          fits inside a square of side n.
 * https://oeis.org/A368638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=1..n} binomial(n-i+2,2)*phi(i)
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* compute Euler's totient values for 0..n */
static seq_t *compute_phi_array(idx_t n)
{
    seq_t *phi = mem_new(seq_t, (n + 1));
    if (!phi) return NULL;
    for (idx_t i = 0; i <= n; ++i) phi[i] = (seq_t)i;

    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime) { mem_free(phi); return NULL; }

    memset(is_prime, 1, (n + 1) * sizeof(unsigned char));
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p; m <= n; m += p) {
                if (m > p) is_prime[m] = 0;
                phi[m] -= phi[m] / (seq_t)p;
            }
        }
    }
    free(is_prime);
    return phi;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t *phi = compute_phi_array(n);
    if (!phi) return 0; /* allocation failure – unlikely */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t phi_i = phi[i];
        seq_t bin = ((seq_t)(n - i + 2) * (seq_t)(n - i + 1)) / 2;
        total += bin * phi_i;
    }
    mem_free(phi);
    return total + 1;
}
