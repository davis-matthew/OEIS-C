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
 * Helpers:   compute_phi_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/compute_phi_array.h"

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
    free(phi);
    return total + 1;
}
