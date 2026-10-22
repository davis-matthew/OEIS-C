/*
 * A328238  Quotient A003415(n*m)/A003415(n) for the least m > 1 for which
 *          such a quotient is an integer. Here A003415(x) is the arithmetic
 *          derivative of x.
 * https://oeis.org/A328238
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n*A328236(n)) / A003415(n)
 * Range:     u8 n <= 14, u16 n <= 1272, u32 n <= 9304, u64 n <= 9304,
 *            u128 n <= 9304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Factorize a 64‑bit integer: primes[] holds the distinct primes,
   exps[] holds their exponents, cnt returns the number of factors.  */
static void factor_uint64(uint64_t x, uint64_t primes[], uint64_t exps[], int *cnt)
{
    int c = 0;
    if (x == 1) {
        *cnt = 0;
        return;
    }
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (x > 1) {
        primes[c] = x;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t N = (seq_t)n;          /* n cast to the arithmetic type               */
    uint64_t n_val = (uint64_t)n;

    /* Factorisation of n */
    uint64_t primesN[64];
    uint64_t expsN[64];
    int cntN;
    factor_uint64(n_val, primesN, expsN, &cntN);

    /* Arithmetic derivative of n:  d(n) = n * Σ(e_i / p_i)  */
    seq_t d_n = 0;
    for (int i = 0; i < cntN; ++i) {
        seq_t term = (N / primesN[i]) * expsN[i];
        d_n += term;
    }

    /* Search for the smallest m > 1 such that d(n*m) is a multiple of d(n)  */
    for (uint64_t m = 2; ; ++m) {
        seq_t mn = N * (seq_t)m;        /* n * m in arithmetic type */

        /* Factorisation of m */
        uint64_t primesM[64];
        uint64_t expsM[64];
        int cntM;
        factor_uint64(m, primesM, expsM, &cntM);

        /* Compute d(n*m) by combining the prime data of n and m */
        seq_t d_nm = 0;

        /* Contributions from primes that already divide n */
        for (int i = 0; i < cntN; ++i) {
            uint64_t p   = primesN[i];
            uint64_t e_m = 0;        /* exponent of p in m (0 if absent)       */
            for (int j = 0; j < cntM; ++j)
                if (primesM[j] == p) { e_m = expsM[j]; break; }

            uint64_t total_exp = expsN[i] + e_m;
            seq_t term = (mn / p) * total_exp;
            d_nm += term;
        }

        /* Primes that appear only in m */
        for (int j = 0; j < cntM; ++j) {
            uint64_t q = primesM[j];
            bool inN = false;
            for (int i = 0; i < cntN; ++i)
                if (primesN[i] == q) { inN = true; break; }

            if (!inN) {
                seq_t term = (mn / q) * expsM[j];
                d_nm += term;
            }
        }

        if (d_nm % d_n == 0)    /* first m satisfying the condition */
            return d_nm / d_n;
    }
    /* Unreachable */
    return 0;
}
