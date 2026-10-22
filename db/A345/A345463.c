/*
 * A345463  Number of n X n matrices over GF(2) whose characteristic
 *          polynomial is irreducible.
 * https://oeis.org/A345463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001037(n) * A002884(n) / A000225(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Möbius function for 64‑bit integers (0, ±1). */
static int mu_u64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t primes[128], exps[128];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1)          /* not square‑free */
            return 0;
    return (cnt & 1) ? -1 : 1;    /* parity of number of distinct primes */
}

/* A001037(n): number of monic irreducible polynomials over GF(2) of degree n.
   Formula: (1/n) * Σ_{d|n} μ(d) * 2^{n/d}. */
static seq_t irreducible_count(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            int mu = mu_u64((uint64_t)d);
            if (mu != 0) {
                seq_t term = seq_pow((seq_t)2, (idx_t)(n / d));
                sum += (seq_t)mu * term;
            }
        }
    }
    return sum / (seq_t)n;   /* division is exact */
}

/* Sequence A345463: number of n×n matrices over GF(2) with irreducible characteristic polynomial */
seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2_n = seq_pow((seq_t)2, (idx_t)n);

    /* A001037(n) */
    seq_t irr = irreducible_count(n);

    /* product_{k=1}^{n-1} (2^n - 2^k)  (GL_n(2) divided by 2^n-1) */
    seq_t prod = 1;
    seq_t cur  = seq_pow((seq_t)2, 1);   /* 2^1 */

    for (idx_t k = 1; k < n; ++k) {
        seq_t term = pow2_n - cur;
        prod = seq_mul(prod, term);
        cur  = seq_mul(cur, 2);          /* next power of 2 */
    }

    return seq_mul(irr, prod);
}
