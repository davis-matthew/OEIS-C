/*
 * A372305  a(n) = Product_{k=2..n-1} MultiplicativeOrder(k,n) where
 *          gcd(k,n)=1.
 * https://oeis.org/A372305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=2..n-1, gcd(k,n)=1} Ord(k,n) (Ord is the
 *            multiplicative order of k mod n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 27,
 *            big any n
 * Helpers:   factor_u64, gcd, mod_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include "../utils/gcd.h"                /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/mod_pow.h"            /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/seq_mul.h"            /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/factor_u64.h"         /* size_t factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

/* Helper: compute lcm of two uint64_t values using seq_t gcd. */
static uint64_t lcm_u64(uint64_t a, uint64_t b)
{
    uint64_t g = (uint64_t)gcd((seq_t)a, (seq_t)b);
    return a / g * b;
}

/* Helper: compute the Carmichael λ(n) from prime factors. */
static uint64_t carmichael_lambda(uint64_t n, uint64_t *pr, uint64_t *ex, size_t cnt)
{
    uint64_t lam = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        uint64_t comp;
        if (p == 2) {
            if (e == 1) comp = 1;
            else if (e == 2) comp = 2;
            else comp = 1ULL << (e - 2);            /* 2^(e-2) for e>=3 */
        } else {
            uint64_t powp = 1;
            for (uint64_t j = 1; j < e; ++j) powp *= p;  /* p^(e-1) */
            comp = (p - 1) * powp;
        }
        lam = lcm_u64(lam, comp);
    }
    return lam;
}

/* Helper: generate all divisors of m given its prime factorisation.
   The output array must have enough space for at most (e1+1)*(e2+1)*... elements. */
static size_t divisors_of(uint64_t m, uint64_t *pr, uint64_t *ex, size_t cnt, uint64_t *out, size_t maxcnt)
{
    size_t pos = 0;
    out[pos++] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        size_t oldcnt = pos;
        uint64_t powp = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            powp *= p;
            for (size_t j = 0; j < oldcnt; ++j) {
                if (pos < maxcnt) out[pos++] = out[j] * powp;
            }
        }
    }
    /* sort ascending */
    for (size_t i = 1; i < pos; ++i) {
        uint64_t key = out[i];
        size_t j = i;
        while (j > 0 && out[j - 1] > key) {
            out[j] = out[j - 1];
            --j;
        }
        out[j] = key;
    }
    return pos;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    /* Base cases: empty product. */
    if (nn <= 2) {
        return (seq_t)1;
    }

    /* 1. Factor n. */
    uint64_t primes[32], exps[32];
    size_t nf = 0;
    factor_u64(nn, primes, exps, &nf);

    /* 2. Compute λ(n). */
    uint64_t lam = carmichael_lambda(nn, primes, exps, nf);

    /* 3. Factor λ(n) to obtain its divisors. */
    uint64_t lam_pr[32], lam_ex[32];
    size_t lcnt = 0;
    factor_u64(lam, lam_pr, lam_ex, &lcnt);

    /* Buffer large enough: λ ≤ n ≤ 10000, so at most (e1+1)*(e2+1)*... < 200. */
    uint64_t divs[256];
    size_t dcnt = divisors_of(lam, lam_pr, lam_ex, lcnt, divs, 256);

    /* 4. Iterate over coprime k and multiply their multiplicative orders. */
    seq_t prod = (seq_t)1;
    const seq_t ONE = (seq_t)1;

    for (uint64_t k = 2; k < nn; ++k) {
        /* skip if not coprime */
        if (gcd((seq_t)k, (seq_t)nn) != ONE) {
            continue;
        }

        /* smallest divisor d of λ such that k^d ≡ 1 (mod n) is the order. */
        seq_t order = (seq_t)lam;   /* default to λ */
        for (size_t j = 0; j < dcnt; ++j) {
            uint64_t d = divs[j];
            if (mod_pow((seq_t)k, d, (seq_t)nn) == ONE) {
                order = (seq_t)d;
                break;
            }
        }

        prod = seq_mul(prod, order);
    }

    return prod;
}
