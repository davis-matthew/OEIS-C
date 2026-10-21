/*
 * A260881  Number of trapezoidal words of length n.
 * https://oeis.org/A260881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 * + Sum_{i=1..n} (n-i+1) * phi(i) * +
 *            Sum_{i=0..floor((n-4)/2)} 2*(n-2*i-3) * phi(i+2) * where phi is
 *            the Euler totient function. * This implementation precomputes
 *            phi values up to the * largest n requested and reuses them for
 *            subsequent calls
 * Range:     u8 n <= 11, u16 n <= 74, u32 n <= 3044, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _ensure_phi_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_ensure_phi_upto.h"

seq_t a(idx_t n)
{
    /* static cache of φ values */
    static seq_t *phi_cache = NULL;
    static idx_t  cache_len = 0;

    /* ensure φ is computed up to n */
    _ensure_phi_upto(n, &phi_cache, &cache_len);
    if (!phi_cache) return (seq_t)0;  /* fallback if memory failed */

    seq_t result = 1;  /* initial constant term */

    /* first sum: Σ_{i=1..n} (n-i+1) * φ(i) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)(n - i + 1);
        result += mult * phi_cache[i];
    }

    /* second sum: Σ_{i=0..floor((n-4)/2)} 2*(n-2*i-3)*φ(i+2) */
    if (n >= 4) {
        idx_t max_i = (n - 4) / 2;
        for (idx_t i = 0; i <= max_i; ++i) {
            seq_t factor = 2 * (seq_t)(n - 2 * i - 3);
            result += factor * phi_cache[i + 2];
        }
    }

    return result;
}
