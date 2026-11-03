/*
 * A325817  a(n) is the least k >= 0 such that n-k and n-(sigma(n)-k) are
 *          relatively prime.
 * https://oeis.org/A325817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k >=0 | gcd(n-k, n-(sigma(n)-k))=1}
 * Range:     u8 n <= 494, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8923 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safeguard – sequence is defined for n >= 1 */
    seq_t s = sigma_of(n);
    for (idx_t k = 0; k <= n; ++k) {          /* a(n) <= n-1, but loop up to n for safety */
        seq_t a_val = (seq_t)n - (seq_t)k;      /* non‑negative */
        int64_t diff = (int64_t)n - (int64_t)s + (int64_t)k;  /* may be negative */
        seq_t b_val = (diff >= 0 ? (seq_t)diff : (seq_t)(-diff));
        if (gcd(a_val, b_val) == 1) {
            return (seq_t)k;
        }
    }
    /* Fallback – should not happen because a(n) <= n-1 */
    return (seq_t)n - 1;
}
