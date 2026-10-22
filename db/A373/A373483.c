/*
 * A373483  a(n) = 1 if A083345(n) and A276085(n) are both multiples of 3,
 *          otherwise 0, where A276085 is fully additive with a(p) = p#/p, and
 *          A083345 is the numerator of the fully additive function with a(p)
 *          = 1/p.
 * https://oeis.org/A373483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff both A083345(n) and A276085(n) are multiples of 3,
 *            otherwise 0. Computes the conditions using only modular
 *            arithmetic modulo 3 and 9
 * Range:     u8 n <= 9471, u16 n <= 9471, u32 n <= 9471, u64 n <= 9471,
 *            u128 n <= 9471, big any n
 * Helpers:   _factorize_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_factorize_uint64.h"

seq_t a(idx_t n)
{
    /* n < 1 is undefined, return 0 for safety */
    if (n < 1)
        return (seq_t)0;
    /* a(1) is always 1 (both functions equal 0) */
    if (n == 1)
        return (seq_t)1;

    /* factor up to 64 bits */
    uint64_t primes[64];
    int exps[64];
    size_t pcnt;
    _factorize_uint64((uint64_t)n, primes, exps, &pcnt);

    /* exponents of 2 and 3 for A276085 condition */
    int e2 = 0, e3 = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        if (primes[i] == 2ULL)
            e2 = exps[i];
        else if (primes[i] == 3ULL)
            e3 = exps[i];
    }

    /* A276085(n) mod 3 = e2 + 2*e3 (mod 3) */
    if (((e2 + 2 * e3) % 3) != 0)
        return (seq_t)0;              /* not a multiple of 3 */

    /* Prepare values for A083345 test */
    /* These only involve distinct primes, so we process once */
    int D_mod3 = 1;   /* product of distinct primes modulo 3 (excluding 3) */
    int Q_mod3 = 1;   /* product of distinct primes ≠ 3 modulo 3 */
    int Q_mod9 = 1;   /* product of distinct primes ≠ 3 modulo 9 */
    bool div_by3 = (e3 > 0);

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int p_mod3 = (int)(p % 3ULL);
        int p_mod9 = (int)(p % 9ULL);
        if (p != 3ULL) {
            D_mod3 = (int)((D_mod3 * p_mod3) % 3);
            Q_mod3 = (int)((Q_mod3 * p_mod3) % 3);
            Q_mod9 = (int)((Q_mod9 * p_mod9) % 9);
        }
    }

    /* A083345 condition */
    if (!div_by3) {                 /* n not divisible by 3 */
        int N_mod3 = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            int exp_mod3 = exps[i] % 3;
            /* inverse of p mod 3: 1→1, 2→2 (since 2*2≡1 mod3) */
            int inv = (int)((p % 3ULL == 1ULL) ? 1 : 2);
            int d_over_p = (int)((D_mod3 * inv) % 3);
            int term = (int)((exp_mod3 * d_over_p) % 3);
            N_mod3 = (N_mod3 + term) % 3;
        }
        if (N_mod3 == 0)
            return (seq_t)1;
        else
            return (seq_t)0;
    } else {                         /* n divisible by 3 */
        int K_mod3 = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            if (p == 3ULL)
                continue;
            int exp_mod3 = exps[i] % 3;
            int inv = (int)((p % 3ULL == 1ULL) ? 1 : 2);
            int term = (int)((Q_mod3 * inv) % 3);   /* Q/p mod 3 */
            K_mod3 = (int)((K_mod3 + exp_mod3 * term) % 3);
        }
        int N_mod9 = (int)(((e3 % 9) * (Q_mod9 % 9)) % 9);
        N_mod9 = (int)((N_mod9 + 3 * (K_mod3 % 3)) % 9);
        if (N_mod9 == 0)
            return (seq_t)1;
        else
            return (seq_t)0;
    }
}
