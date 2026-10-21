/*
 * A246260  a(n) = 1 if A003961(n) is of the form 4k+1, otherwise a(n) = 0,
 *          (when A003961(n) is of the form 4k+3). [A003961 is fully
 *          multiplicative with a(p) = nextprime(p)].
 * https://oeis.org/A246260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003961(n) ≡ 1 (mod 4), otherwise 0; * Here
 *            A003961(n) is fully multiplicative with a(p)=nextprime(p). * The
 *            result is 1 iff the number of prime factors p with *
 *            nextprime(p) ≡ 3 (mod 4) that occur with odd exponent in n * is
 *            even. This computation factors n, checks each prime factor,...
 * Range:     u8 n <= 9717, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour for 0 is irrelevant */
    uint64_t nn = (uint64_t)n;
    if (nn == 1) return (seq_t)1;          /* A003961(1)=1 → 1 mod 4 */

    /* Maximum distinct prime factors of a 64‑bit integer is < 64   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    bool flip = false;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);   /* nextprime(p) */
        /* If nextprime(p) ≡ 3 (mod 4) and exponent e is odd, toggle flag */
        if (((q & 3ULL) == 3ULL) && (e & 1ULL)) {
            flip = !flip;
        }
    }
    return flip ? (seq_t)0 : (seq_t)1;
}
