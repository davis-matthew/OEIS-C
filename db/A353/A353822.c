/*
 * A353822  Product_{n>=1} (1 + x^n/n!)^a(n) = exp(-x)/(1 - x).
 * https://oeis.org/A353822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 *?
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binom, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>=2,
 * a(n)= (n-1)! - ∑_{d|n, d<n} a(d) * (-1)^{n/d+1} * multinomial(n; d,...,d)/ (n/d)      */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/binom.h"     /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/factorial.h" /* seq_t factorial(idx_t n) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

static seq_t *fac = NULL;          /* factorial array: fac[i] = i! */
static size_t fac_cap = 0;          /* capacity of fac */
static idx_t  fac_len = 0;          /* last filled index (inclusive) */

static seq_t *A   = NULL;          /* sequence array: A[i] = a(i) */
static size_t Acap = 0;            /* capacity of A */
static idx_t  Alen = 0;            /* last filled index (inclusive) */

/* ensure factorial array up to n inclusive */
static void ensure_factorial(idx_t n)
{
    if (fac_len > n) return;
    size_t new_cap = fac_cap ? fac_cap : 1;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;

    if (!fac_cap) {
        fac = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!fac) return;
        fac[0] = 1;
        fac_len = 0;
        fac_cap = new_cap;
    } else {
        fac = (seq_t*)realloc(fac, new_cap * sizeof(seq_t));
        fac_cap = new_cap;
    }

    for (idx_t i = fac_len + 1; i <= n; ++i)
        fac[i] = seq_mul(fac[i-1], (seq_t)i);
    fac_len = n;
}

/* ensure sequence array up to n inclusive, computing values by recurrence */
static void ensure_sequence(idx_t n)
{
    if (Alen > n) return;
    size_t new_cap = Acap ? Acap : 1;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;

    if (!Acap) {
        A = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!A) return;
        Acap = new_cap;
        /* initialise first value */
        A[1] = 0;
        Alen = 1;
    } else {
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        Acap = new_cap;
    }

    for (idx_t m = Alen + 1; m <= n; ++m) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* iterate over all proper divisors d of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d != 0) continue;
            /* first divisor d */
            if (d < m) {
                idx_t k = m / d; /* k = m/d, k >= 2 */
                /* compute multinomial (m!)/(d!)^k using product of binomials */
                seq_t mult = 1;
                idx_t remaining = m;
                while (remaining > 0) {
                    mult = seq_mul(mult, binom(remaining, d));
                    remaining -= d;
                }
                /* divide by k */
                seq_t factor = mult / (seq_t)k;
                seq_t term   = seq_mul(A[d], factor);
                if ((k & 1) == 0)      /* k even => sign = -1 */
                    sum_neg = sum_neg + term;
                else
                    sum_pos = sum_pos + term;
            }
            /* second divisor, if different */
            idx_t d2 = m / d;
            if (d2 != d && d2 < m) {
                idx_t k = m / d2;
                seq_t mult = 1;
                idx_t remaining = m;
                while (remaining > 0) {
                    mult = seq_mul(mult, binom(remaining, d2));
                    remaining -= d2;
                }
                seq_t factor = mult / (seq_t)k;
                seq_t term   = seq_mul(A[d2], factor);
                if ((k & 1) == 0)
                    sum_neg = sum_neg + term;
                else
                    sum_pos = sum_pos + term;
            }
        }

        A[m] = fac[m-1] + sum_neg - sum_pos;
    }

    Alen = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_factorial(n);
    ensure_sequence(n);
    return A[n];
}
