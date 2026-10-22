/*
 * A377874  Parity of A083345(n), where A083345(n) = n' / gcd(n,n') =
 *          numerator of Sum(e/p: n=Product(p^e)).
 * https://oeis.org/A377874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the parity of the reduced numerator of Σ e/p where n = Π
 *            p^e. Specifically, a(n) = 1 if the reduced numerator is odd, 0
 *            otherwise
 * Range:     u8 n <= 9922, u16 n <= 9922, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition (empty sum). */
    if (n == 1) return (seq_t)0;

    /* Simple trial‑division factorisation of n. */
    idx_t tmp = n;
    idx_t primes[32];
    int   exps [32];
    int    cnt  = 0;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps  [cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        primes[cnt] = tmp;
        exps  [cnt] = 1;
        ++cnt;
    }

    /* Compute P = product of distinct primes of n. */
    uint64_t P = 1;
    for (int i = 0; i < cnt; ++i)
        P *= (uint64_t)primes[i];

    /* Compute N0 = Σ e_i * (P / p_i).  All values fit in 64 bits for n ≤ 10⁴. */
    uint64_t N0 = 0;
    for (int i = 0; i < cnt; ++i)
        N0 += (uint64_t)exps[i] * (P / (uint64_t)primes[i]);

    /* v2(N0): exponent of 2 dividing N0. */
    int v2N0 = 0;
    uint64_t t = N0;
    while ((t & 1ULL) == 0) {
        ++v2N0;
        t >>= 1;
    }

    /* v2(P) is 1 if n has factor 2, else 0. */
    int v2P = ( (n & 1ULL) == 0 ) ? 1 : 0;

    /* Reduced numerator is odd iff v2(N0) ≤ v2(P). */
    int parity = (v2N0 <= v2P) ? 1 : 0;
    return (seq_t)parity;
}
