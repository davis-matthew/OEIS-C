/*
 * A369742  a(n) = Sum_{p|n, p prime} p^(mu(n/p)^2).
 * https://oeis.org/A369742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^{mu(n/p)^2}
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = p if dividing n by p leaves a square‑free number,
 *  otherwise a(n) contributes 1.
 *
 *  The implementation factorises n by trial division.
 *  For each prime factor p^e we check the exponent count of other primes:
 *     - if e >= 3          → n/p has a square factor → add 1
 *     - if e == 2          → n/p is square‑free iff n has
 *                            exactly one prime with exponent > 1   → add p
 *                            otherwise add 1
 *     - if e == 1          → n/p is square‑free iff n is square‑free
 *                            → add p if no prime exponent > 1, else add 1
 */

/* Standard integer types */
#include <stdint.h>
#include <stddef.h>

/* idx_t and seq_t are defined by the build system */

seq_t a(idx_t n)
{
    /* Offset handling */
    if (n == 1)
        return 0;

    /* Prime factorisation of n (trial division) */
    uint64_t primes[64];
    uint32_t exps[64];
    size_t pc = 0;

    uint64_t temp = n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[pc] = temp;
        exps[pc]   = 1;
        ++pc;
    }

    /* Count how many primes have exponent > 1 in n */
    size_t c2 = 0;
    for (size_t i = 0; i < pc; ++i)
        if (exps[i] > 1)
            ++c2;

    /* Sum contributions */
    seq_t sum = 0;
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        uint32_t e = exps[i];

        if (e >= 3) {
            sum += (seq_t)1;
        } else if (e == 2) {
            if (c2 == 1)          /* only this prime had exponent > 1 */
                sum += (seq_t)p;
            else
                sum += (seq_t)1;
        } else { /* e == 1 */
            if (c2 == 0)          /* n itself was square‑free */
                sum += (seq_t)p;
            else
                sum += (seq_t)1;
        }
    }

    return sum;
}
