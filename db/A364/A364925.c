/*
 * A364925  If the Sylow 2-subgroup of the multiplicative group modulo n is
 *          isomorphic to the direct product of (C_(2^i))^(e_i) for 1 <= i <=
 *          k, where C_m denotes the cyclic group of order m, then a(n) =
 *          Product_{i=1..k} prime(i)^(e_i).
 * https://oeis.org/A364925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 5439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364925: Sylow 2‑subgroup product of prime indices
   a(n) = Π prime(i)^(e_i) where e_i = multiplicity of C_(2^i) in the Sylow 2‑subgroup of (ℤ/nℤ)^*
   (based on the OEIS description). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1, guard anyway */

    uint64_t m = (uint64_t)n;
    /* counts for components of size 2^i, i >= 1 */
    unsigned int cnt[65] = {0};

    /* 2‑part of n */
    unsigned int e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2 == 2) {
        cnt[1]++;                      /* (ℤ/4ℤ)^* ≅ C2 */
    } else if (e2 >= 3) {
        cnt[1]++;                      /* C2 factor */
        cnt[e2 - 2]++;                 /* C_{2^{e2-2}} factor */
    }
    /* odd part */
    if (m > 1) {
        uint64_t primes[64];
        int exps[64];
        size_t num = factorize(m, primes, exps);
        for (size_t i = 0; i < num; ++i) {
            uint64_t p = primes[i];
            /* p is odd */
            uint64_t t = p - 1;
            unsigned int v = 0;
            while ((t & 1ULL) == 0) {
                ++v;
                t >>= 1;
            }
            /* v >= 1 for odd prime */
            cnt[v]++;
        }
    }
    /* product over i */
    seq_t result = 1;
    for (unsigned int i = 1; i < 65; ++i) {
        unsigned int c = cnt[i];
        if (c == 0) continue;
        seq_t prime_i = get_prime((idx_t)i);
        seq_t pow = 1;
        for (unsigned int k = 0; k < c; ++k) pow *= prime_i;
        result *= pow;
    }
    return result;
}
