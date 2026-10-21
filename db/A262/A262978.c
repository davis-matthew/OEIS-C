/*
 * A262978  Exponents n such that 2^n-1 and 2^n+1 are squarefree.
 * https://oeis.org/A262978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th exponent e such that 2^e−1 and 2^e+1 are * both
 *            squarefree. For n ≥ 1, a(n) is defined as the n‑th such *
 *            exponent in increasing order. The implementation precomputes all
 *            * admissible exponents up to a fixed bound (200 000) by sieving
 *            with * the orders of 2 modulo p² for all odd p...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 608, u64 n <= 608, u128 n <= 608,
 *            big any n
 * Helpers:   build_primes, build_spf, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes.h"
#include "../utils/build_spf.h"
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* ---------- static data ---------------------------------------------------- */
    static bool initialized = false;
    static seq_t *allowed = NULL;      /* array of admissible exponents      */
    static size_t allowed_len = 0;     /* number of stored exponents         */

    /* ---------- precomputation (first call) ----------------------------------- */
    if (!initialized) {
        /* ---- parameters ----------------------------------------------------- */
        const idx_t LIMIT_EXP   = 200000;          /* maximum exponent to examine   */
        const idx_t LIMIT_PRIME = LIMIT_EXP + 10;  /* primes up to this limit       */

        /* ---- generate primes up to LIMIT_PRIME ------------------------------ */
        idx_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes(LIMIT_PRIME, &primes, &prime_cnt);

        /* ---- smallest‑prime‑factor table for numbers up to LIMIT_PRIME ----- */
        idx_t *spf = NULL;
        build_spf(LIMIT_PRIME, &spf);

        /* ---- array marking forbidden exponents ------------------------------ */
        unsigned char *forbidden = (unsigned char *)
                                   calloc((size_t)(LIMIT_EXP + 1), sizeof(unsigned char));
        if (!forbidden) return (seq_t)0;          /* unlikely in tests but safe  */

        /* ---- process each odd prime ------------------------------------------ */
        for (size_t i = 1; i < prime_cnt; ++i) {   /* primes[0] is 2, skip it        */
            idx_t p = primes[i];                  /* odd prime                     */
            idx_t mod = p * p;                    /* p² < 4·10¹⁰ fits in 64‑bit   */

            /* minimal order d of 2 modulo p² (initially φ(p²) = p·(p−1))        */
            idx_t d = p * (p - 1);

            /* collect prime factors of d (unique)                                 */
            idx_t factors[16];
            size_t fcnt = 0;
            factors[fcnt++] = p;                   /* factor p                     */
            idx_t tmp = p - 1;
            while (tmp > 1) {                      /* factor p−1 using spf         */
                idx_t f = spf[tmp];
                bool is_new = true;
                for (size_t z = 0; z < fcnt; ++z)
                    if (factors[z] == f) { is_new = false; break; }
                if (is_new) factors[fcnt++] = f;
                while (tmp % f == 0) tmp /= f;
            }

            /* reduce d by removing factors while 2^(d/q) ≡ 1 (mod p²)            */
            for (size_t z = 0; z < fcnt; ++z) {
                idx_t f = factors[z];
                while (d % f == 0) {
                    idx_t cand = d / f;
                    if (mod_pow((seq_t)2, cand, (seq_t)mod) == (seq_t)1)
                        d = cand;
                    else
                        break;
                }
            }

            /* mark multiples of d as forbidden (2^k−1 divisible by p²)          */
            for (idx_t k = d; k <= LIMIT_EXP; k += d)
                forbidden[(size_t)k] = 1;

            /* if d even, test 2^(d/2) ≡ −1 (mod p²)                            */
            idx_t d_neg = 0;
            if ((d & 1ULL) == 0) {
                if (mod_pow((seq_t)2, d >> 1, (seq_t)mod) == (seq_t)(mod - 1))
                    d_neg = d >> 1;
            }

            /* mark odd multiples of d_neg as forbidden (2^k+1 divisible by p²)   */
            if (d_neg != 0) {
                for (idx_t k = d_neg; k <= LIMIT_EXP; k += d_neg) {
                    if (((k / d_neg) & 1ULL) != 0)
                        forbidden[(size_t)k] = 1;
                }
            }
        }   /* end for primes */

        /* ---- collect all allowed exponents --------------------------------- */
        allowed = (seq_t *)malloc((size_t)LIMIT_EXP * sizeof(seq_t));
        if (!allowed) return (seq_t)0;  /* unlikely but safe              */
        size_t cnt = 0;
        for (idx_t exp = 1; exp <= LIMIT_EXP; ++exp) {
            if (!forbidden[(size_t)exp]) {
                allowed[cnt++] = (seq_t)exp;
            }
        }
        allowed_len = cnt;

        /* ---- free temporaries ------------------------------------------------ */
        free(forbidden);
        free(primes);
        free(spf);

        initialized = true;
    }   /* end if !initialized */

    /* ---------- return the requested n‑th term ------------------------------- */
    if (n == 0 || (idx_t)n > allowed_len)
        return (seq_t)0;                  /* fallback for out‑of‑range */
    return allowed[(size_t)(n - 1)];
}
