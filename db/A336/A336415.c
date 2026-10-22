/*
 * A336415  Number of divisors of n! with equal prime multiplicities.
 * https://oeis.org/A336415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - Emax + Σ_{e=1}^{Emax} 2^{m_e} where m_e = number of
 *            primes p ≤ n with exponent in n! at least e
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset terms */
    if (n <= 1)
        return (seq_t)1;

    /* --------- Generate primes ≤ n with a simple sieve --------- */
    idx_t limit = n;
    bool *is_comp = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_comp)
        return (seq_t)0;                       /* OOM fallback */
    memset(is_comp, 0, (size_t)(limit + 1) * sizeof(bool));

    size_t primes_cap = 10;
    size_t primes_n = 0;
    size_t *primes = (size_t *)malloc(primes_cap * sizeof(size_t));
    if (!primes) { free(is_comp); return (seq_t)0; }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_n >= primes_cap) {
                primes_cap = primes_cap * 2 + 10;
                size_t *tmp = (size_t *)realloc(primes, primes_cap * sizeof(size_t));
                if (!tmp) { free(is_comp); free(primes); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_n++] = (size_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* --------- Compute vp(n!) for each prime and collect exponents --------- */
    idx_t *exps = (idx_t *)malloc(primes_n * sizeof(idx_t));
    if (!exps) { free(primes); return (seq_t)0; }

    for (size_t ii = 0; ii < primes_n; ++ii) {
        idx_t p = (idx_t)primes[ii];
        idx_t exp = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            exp += t;
        }
        exps[ii] = exp;
    }

    /* --------- Determine maximum exponent and build frequency table --------- */
    idx_t Emax = 0;
    for (size_t ii = 0; ii < primes_n; ++ii)
        if (exps[ii] > Emax)
            Emax = exps[ii];

    size_t *freq = (size_t *)calloc((size_t)(Emax + 1), sizeof(size_t));
    if (!freq) { free(primes); free(exps); return (seq_t)0; }

    for (size_t ii = 0; ii < primes_n; ++ii)
        freq[(size_t)exps[ii]]++;

    /* --------- Compute Σ 2^{m_e} ------------------------------------------- */
    seq_t S = (seq_t)0;
    idx_t cur = 0;          /* number of primes with exponent ≥ current k */
    seq_t pow2_cur = (seq_t)1;  /* 2^0 */

    for (idx_t k = Emax; k >= 1; --k) {
        cur += freq[(size_t)k];          /* m_k */
        seq_t mult = seq_pow((seq_t)2, (idx_t)freq[(size_t)k]); /* 2^{freq[k]} */
        pow2_cur = pow2_cur * mult;        /* 2^{cur} */
        S += pow2_cur;
    }

    /* --------- Final result ----------------------------------------------- */
    seq_t result = (seq_t)1 + S - (seq_t)Emax;

    /* --------- Clean up ----------------------------------------------------- */
    free(exps);
    free(freq);
    free(primes);

    return result;
}
