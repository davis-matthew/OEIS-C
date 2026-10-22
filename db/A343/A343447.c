/*
 * A343447  Smallest m such that alternating integer 101...101 = A094028(m) is
 *          a multiple of A045572(n), (i.e., integers coprime with 10).
 * https://oeis.org/A343447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = minimal m ≥ 0 such that Σ_{i=0..m} 10^{2i} ≡ 0 (mod
 *            A045572(n)) Sequence: Smallest m such that the alternating
 *            integer 101…101 (=A094028(m)) is a multiple of the n-th integer
 *            coprime to 10 (A045572(n)). a(1) = 0 by convention (1 divides
 *            everything)
 * Range:     u8 n <= 5, u16 n <= 949, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small, nth_coprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/nth_coprime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n<1 is undefined, return 0 harmlessly */
    seq_t k = nth_coprime(n);              /* k = A045572(n) */
    if (k == (seq_t)1) return (seq_t)0;    /* by definition a(1)=0 */

    seq_t r = 1 % k;                       /* S_0 = 1 */
    seq_t pow = 1 % k;                     /* 10^0 mod k */

    idx_t m = 1;
    for (;; ++m) {
        /* pow = 10^{2m} mod k  (each iteration multiplies by 100) */
        pow = mul_small(pow, 100) % k;     /* mul_small works for built‑in or big integers */
        r = (r + pow) % k;                 /* S_m = S_{m-1} + 10^{2m} mod k */
        if (r == 0) return (seq_t)m;
    }
}
