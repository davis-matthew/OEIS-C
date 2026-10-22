/*
 * A342001  Arithmetic derivative without its inherited divisor; the
 *          arithmetic derivative of n divided by A003557(n), which is a
 *          common divisor of both n and A003415(n).
 * https://oeis.org/A342001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A342001 = Σ_{p^v || n} v * (rad(n)/p) where rad(n) = Π_{p
 *            | n} p and the sum is over all distinct prime divisors p of n
 *            with exponent v. This follows from: n' = n * Σ v/p and a(n) =
 *            n'/gcd(n,n') which leads to a(n) = rad(n)*Σ v/p = Σ v*rad(n)/p
 * Range:     u8 n <= 301, u16 n <= 9518, u32 n <= 9518, u64 n <= 9518,
 *            u128 n <= 9518, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* a(1) = 0 */

    /* ------------------------------------------------------------------ */
    /* factor n: store prime factors and their exponents                 */
    /* ------------------------------------------------------------------ */
    const size_t MAX_FACTORS = 64;       /* enough for any 64‑bit n */
    idx_t  primes[MAX_FACTORS];
    idx_t  exps  [MAX_FACTORS];
    size_t fcnt  = 0;

    idx_t t = n;                         /* temporary copy for factorization */
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (fcnt < MAX_FACTORS) {
                primes[fcnt] = p;
                exps[fcnt]   = e;
                ++fcnt;
            }
        }
    }
    if (t > 1 && fcnt < MAX_FACTORS) {   /* remaining prime factor */
        primes[fcnt] = t;
        exps[fcnt]   = (idx_t)1;
        ++fcnt;
    }

    /* ------------------------------------------------------------------ */
    /* compute rad(n) as product of distinct primes                       */
    /* ------------------------------------------------------------------ */
    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < fcnt; ++i)
        rad *= (seq_t)primes[i];

    /* ------------------------------------------------------------------ */
    /* compute a(n) = Σ v * (rad/p)                                        */
    /* ------------------------------------------------------------------ */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t term = rad / (seq_t)primes[i];      /* rad/p   */
        result += (seq_t)exps[i] * term;          /* v * term*/
    }

    return result;
}
