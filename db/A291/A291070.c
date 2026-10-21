/*
 * A291070  Number of 5 X n binary matrices that are "primitive"; that is,
 *          they cannot be expressed as a "tiling" by a smaller matrix.
 * https://oeis.org/A291070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{d|n} μ(d) * (2^{5·(n/d)} – 2^{n/d})
 * where μ is the Möbius function.
 * This counts 5×n binary matrices that cannot be tiled by a smaller matrix.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t, etc */
#include <stdbool.h>  /* bool */

#include "../utils/pow2.h"   /* pow2(idx_t) -> seq_t */

/* Helper: compute the Möbius value of a number via its
   prime factorisation (we only need μ of all divisors,
   so we use a bitmask over distinct primes). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, but safe guard */

    /* Find distinct prime factors of n. */
    idx_t primes[32];
    size_t pf_cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[pf_cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[pf_cnt++] = temp;   /* remaining prime */

    seq_t result = 0;

    /* Iterate over all square‑free divisors: subsets of distinct primes. */
    size_t subsets = (idx_t)1 << pf_cnt;
    for (size_t mask = 0; mask < subsets; ++mask) {
        /* Current divisor d = product of selected primes. */
        idx_t d = 1;
        int bits = 0;           /* number of primes in divisor */
        for (size_t i = 0; i < pf_cnt; ++i) {
            if (mask & ((size_t)1 << i)) {
                d *= primes[i];
                bits++;
            }
        }

        idx_t exp = n / d;      /* exponent for powers of 2 */

        seq_t t1 = pow2(5 * exp);
        seq_t t2 = pow2(exp);
        seq_t term = t1 - t2;   /* (2^{5·exp} – 2^{exp}) */

        if (bits & 1)           /* μ(d) = -1 if odd number of primes */
            result -= term;
        else                    /* μ(d) = +1 if even (including 0 primes) */
            result += term;
    }

    return result;
}
