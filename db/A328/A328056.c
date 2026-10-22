/*
 * A328056  Numbers k such that phi(k) > phi(k+1) > phi(k+2) where phi is the
 *          Euler totient function (A000010).
 * https://oeis.org/A328056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that phi(k) > phi(k+1) > phi(k+2)
 * Range:     u8 none, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t tested_k = 0;   /* last k that was examined */

    if (n == (idx_t)0) return 0;  /* undefined for n=0, return 0 */

    while (cache_len < (size_t)n) {
        ++tested_k;                 /* next k to check */

        seq_t phi_k   = phi(tested_k);
        seq_t phi_k1  = phi(tested_k + 1);
        seq_t phi_k2  = phi(tested_k + 2);

        if (phi_k > phi_k1 && phi_k1 > phi_k2) {
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)tested_k);
        }
    }
    return cache[(size_t)(n - 1)];
}
