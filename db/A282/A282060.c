/*
 * A282060  Coefficients in q-expansion of E_4*(E_2*E_4 - E_6)/720, where E_2,
 *          E_4, E_6 are the Eisenstein series shown in A006352, A004009,
 *          A013973, respectively.
 * https://oeis.org/A282060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * σ₇(n) (sum of seventh powers of the divisors of n)
 *            For n > 0, a(n) is multiplicative with a(p^e) = p^e * (1 + p⁷ +
 *            p¹⁴ + … + p⁷ˢ) (s = e) a(0) = 0, a(1) = 1. The implementation
 *            factors each n, evaluates the local factor using only 64‑bit
 *            arithmetic for the 64‑bit build, and uses a 128‑...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Helper that safely multiplies two seq_t values.  For 64‑bit builds
   the product is performed as an unsigned __int128 and cast back,
   avoiding overflow.  For arbitrary‑precision builds the ordinary
   multiplication of seq_t is used. */
static inline seq_t mulseq(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    return (sizeof(seq_t) <= 8) ?
        (seq_t)((unsigned __int128)a * (unsigned __int128)b) :
        a * b;
#else
    return a * b;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Factorise n; the arrays are large enough for 64‑bit n. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p      = primes[i];
        uint64_t e      = exps[i];
        seq_t   p_seq   = (seq_t)p;

        /* Compute p^e. */
        seq_t p_pow_e = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j)
            p_pow_e = mulseq(p_pow_e, p_seq);

        /* Compute p^7. */
        seq_t powp7 = (seq_t)1;
        for (int j = 0; j < 7; ++j)
            powp7 = mulseq(powp7, p_seq);

        /* Compute sum_{k=0..e} (p^7)^k = 1 + p^7 + p^14 + … + p^(7e). */
        seq_t sum  = (seq_t)1;
        seq_t term = (seq_t)1;
        for (uint64_t k = 1; k <= e; ++k) {
            term = mulseq(term, powp7);
            sum += term;
        }

        /* a(p^e) = p^e * sum. */
        seq_t a_p = mulseq(p_pow_e, sum);

        /* Accumulate multiplicative result. */
        result = mulseq(result, a_p);
    }

    return result;
}
