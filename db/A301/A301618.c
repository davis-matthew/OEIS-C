/*
 * A301618  Least k > n such that gcd(k+1,n+1) > gcd(k,n) > 1.
 * https://oeis.org/A301618
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n such that gcd(k+1, n+1) > gcd(k, n) > 1
 * Range:     u8 n <= 16, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* value for indices < 2 is irrelevant */

    /* Quick handling for prime indices: a(p) = p * (p + 2) */
    if (is_prime(n))
        return (seq_t)n * ((seq_t)n + 2);

    /* Brute‑force search for composite indices */
    idx_t k = n + 1;
    while (1) {
        seq_t gk  = gcd((seq_t)k, (seq_t)n);
        if (gk <= 1) {               /* skip coprime k */
            ++k;
            continue;
        }
        seq_t gk1 = gcd((seq_t)(k + 1), (seq_t)(n + 1));
        if (gk1 > gk)                /* found the required k */
            return (seq_t)k;
        ++k;
    }
}
