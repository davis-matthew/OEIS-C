/*
 * A265277  E.g.f.: Sum_{n>=0} (n*y + x^n)^n / n! - Sum_{n>=0} n^n*y^n / n! at
 *          y=2.
 * https://oeis.org/A265277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (2*d)^(d - n/d) * binomial(d, n/d) * n!/d! (y =
 *            2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow_seq.h"     /* pow_seq(base, exp) */
#include "../utils/binomial.h"   /* binomial(n, k) */

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* unused, but safe */

    /* ---------- collect all divisors of n -------------------------------- */
    idx_t divs[256];
    size_t divc = 0;
    idx_t sq = (idx_t)((double)n * 0.5) + 1;  /* rough upper bound for sqrt */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i, d2 = n / i;
        if (divc < sizeof(divs)/sizeof(divs[0])) divs[divc++] = d1;
        if (d2 != d1 && divc < sizeof(divs)/sizeof(divs[0])) divs[divc++] = d2;
    }

    /* ---------- pre‑compute suffix products Σ_{k=d+1}^{n} k ---------------- */
    seq_t *suf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!suf) return 0; /* allocation failure */
    suf[n] = 1;                 /* product over empty set */
    for (idx_t i = n; i-- > 0; ) {
        suf[i] = suf[i + 1] * (seq_t)(i + 1);
    }

    /* ---------- sum over divisors --------------------------------------- */
    seq_t sum = 0;
    for (size_t j = 0; j < divc; ++j) {
        idx_t d = divs[j];
        idx_t k = n / d;
        if (k > d) continue;          /* binomial(d, k) would be zero    */
        seq_t bin = binomial(d, k);    /* binomial coefficient            */
        seq_t base = (seq_t)(2 * d);    /* 2*d                              */
        int exp = (int)(d - k);        /* d - n/d (non‑negative)          */
        seq_t pow = pow_seq(base, exp);/* (2*d)^(d-k)                      */
        seq_t prod = suf[d];           /* n!/d!                            */
        sum += bin * pow * prod;       /* accumulate term                 */
    }

    free(suf);
    return sum;
}
