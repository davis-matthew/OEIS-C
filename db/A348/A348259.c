/*
 * A348259  Number of bases 1<b<n and coprime to n, such that b^n == b (mod
 *          n).
 * https://oeis.org/A348259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (∏_{p|n} gcd(p-1, n-1)) - 1
 * Range:     u8 n <= 261, u16 n <= 9138, u32 n <= 9138, u64 n <= 9138,
 *            u128 n <= 9138, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* Returns the nth term of OEIS sequence A348259.
   idx_t and seq_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* n is 1-based. For n = 1 the product over an empty set is 1, so
       a(1) = 1 - 1 = 0, matching the sequence. */
    uint64_t n_minus1 = n - 1;   /* safe: n >= 1 */

    /* Factor n using the provided utility.  The arrays are large
       enough for all 64‑bit numbers. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t prod = 1;   /* starts at multiplicative identity */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* gcd(p-1, n-1) as seq_t */ 
        seq_t g = gcd((seq_t)(p - 1), (seq_t)n_minus1);
        prod *= g;
    }

    /* The count includes b = 1; remove that to get the required count. */
    return prod - 1;
}
