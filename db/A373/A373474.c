/*
 * A373474  a(n) = 1 if A001414(n) and A083345(n) are both multiples of 3,
 *          otherwise 0, where A001414 is fully additive with a(p) = p, and
 *          A083345 is the numerator of the fully additive function with a(p)
 *          = 1/p.
 * https://oeis.org/A373474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1531, u16 n <= 8786, u32 n <= 8786, u64 n <= 8786,
 *            u128 n <= 8786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A373474
   a(n) = 1 if A001414(n) and A083345(n) are both multiples of 3,
          otherwise 0.
   A001414 is fully additive with a(p)=p  →  T(n)=∑ k_p p
   A083345 is the numerator of the fully additive function with a(p)=1/p
   →  S(n)=∑ k_p / p , reduced numerator N
   Condition: T(n)≡0 (mod 3)  and  v₃(N) > v₃(D)
   where D=∏_{p|n} p  (product of distinct primes).
   Since v₃(D) is 1 iff n is divisible by 3, we need
   N≡0 (mod 3)      if 3∤n
   N≡0 (mod 9)      if 3|n
   The value of N modulo 9 can be computed from the prime factorisation
   without forming huge integers.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) {
        /* a(1) = 1 */
        return 1;
    }

    /* Factorise n: store distinct primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    int t = 0;

    uint64_t m = (uint64_t)n;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[t] = d;
            exps[t] = e;
            ++t;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        primes[t] = m;
        exps[t] = 1;
        ++t;
    }

    /* Compute A001414(n) = T = ∑ k_p * p */
    seq_t T = 0;
    for (int i = 0; i < t; ++i) {
        T += (seq_t)exps[i] * primes[i];
    }
    if (T % 3 != 0) {
        return 0;          /* not a multiple of 3 */
    }

    /* Compute v₃(D): D contains 3 iff any prime equals 3 */
    int v3D = 0;
    for (int i = 0; i < t; ++i) {
        if (primes[i] == 3) {
            v3D = 1;
            break;
        }
    }

    /* Compute N modulo 9:
       N = ∑ k_p * (∏_{q ≠ p} q)   (mod 9)   */
    int Nmod9 = 0;
    for (int i = 0; i < t; ++i) {
        /* product of all distinct primes except primes[i] (mod 9) */
        int prod_except = 1;
        for (int j = 0; j < t; ++j) if (j != i) {
            prod_except = (prod_except * (int)(primes[j] % 9)) % 9;
        }
        int exp_mod9 = (int)(exps[i] % 9);
        int term = (exp_mod9 * prod_except) % 9;
        Nmod9 = (Nmod9 + term) % 9;
    }

    /* Check the 3-adic condition */
    bool ok;
    if (v3D == 0) {
        ok = (Nmod9 % 3 == 0);
    } else {
        ok = (Nmod9 % 9 == 0);
    }

    return ok ? 1 : 0;
}
