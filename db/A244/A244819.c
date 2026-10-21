/*
 * A244819  Positive numbers primitively represented by the binary quadratic
 *          form (1, 0, 3).
 * https://oeis.org/A244819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers k = 1, 2, 3, … and keep only those
   numbers whose prime factorisation satisfies  
     • exponent of 2 is 0 or 2,
     • exponent of 3 is at most 1,
     • every other prime factor is ≡ 1 (mod 6).  
   The n‑th such integer is the value a(n).  This implements OEIS A244819. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe */

    uint64_t candidate = 1;
    idx_t    count     = 0;

    for (;; ++candidate) {
        uint64_t x = candidate;
        int e2 = 0, e3 = 0;

        /* factor out powers of 2 */
        while ((x & 1ULL) == 0) {
            ++e2;
            x >>= 1;
        }
        if (e2 != 0 && e2 != 2) continue;   /* exponent of 2 not 0 or 2 */

        /* factor out powers of 3 */
        while (x % 3ULL == 0) {
            ++e3;
            x /= 3ULL;
        }
        if (e3 > 1) continue;               /* exponent of 3 > 1 */

        /* factor remaining part with primes ≡ 1 or 5 (mod 6) */
        bool ok = true;
        for (uint64_t p = 5; (unsigned __int128)p * p <= x; p += 6) {
            if (x % p == 0) {                     /* p ≡ 5 (mod 6) */
                ok = false;
                break;
            }
            uint64_t q = p + 2;                   /* q ≡ 1 (mod 6) */
            if (x % q == 0) {
                /* remove all powers of q */
                while (x % q == 0) x /= q;
            }
        }
        if (!ok) continue;

        /* if any prime factor remains > 1, it must be ≡ 1 (mod 6) */
        if (x > 1 && (x % 6ULL) != 1ULL) continue;

        /* candidate satisfies the condition */
        ++count;
        if (count == n) return (seq_t)candidate;
    }
}
