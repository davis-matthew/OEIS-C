/*
 * A376804  Expansion of 1/((1 - x)^3 - 9*x^3)^(1/3).
 * https://oeis.org/A376804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} (-9)^k * binomial(-1/3,k) *
 *            binomial(n,n-3k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoized array of the coefficients R[k] = 3^k * Π_{i=0}^{k-1}(1+3i)/k! */
static seq_t *R = NULL;
static idx_t Rlen = 0;

/* Ensure the array R contains entries up to and including kmax */
static void ensure_R(idx_t kmax)
{
    if (Rlen > kmax) return;                 /* already long enough */
    size_t newlen = (size_t)kmax + 1;
    seq_t *tmp = (seq_t*)realloc(R, newlen * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failed */
    R = tmp;
    for (idx_t k = Rlen; k <= kmax; ++k) {
        if (k == 0) {
            R[0] = 1;                       /* R[0] = 1 */
        } else {
            /* R[k] = R[k-1] * 3 * (3k-2) / k  (exact integer) */
            seq_t num = R[k-1] * (seq_t)3 * (seq_t)(3*k - 2);
            R[k] = num / (seq_t)k;
        }
    }
    Rlen = newlen;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    idx_t kmax = n / 3;
    ensure_R(kmax);

    seq_t sum  = 0;          /* sum of terms */
    seq_t comb = 1;          /* binomial(n,0) */

    for (idx_t k = 0; k <= kmax; ++k) {
        sum += R[k] * comb;

        if (k == kmax) break;   /* last term processed */

        /* update comb to C(n, 3(k+1)) from C(n, 3k) */
        idx_t num1 = n - 3*k;     /* n - 3k          */
        idx_t num2 = num1 - 1;    /* n - 3k - 1      */
        idx_t num3 = num2 - 1;    /* n - 3k - 2      */
        idx_t den1 = 3*k + 3;     /* 3(k+1)          */
        idx_t den2 = den1 - 1;    /* 3(k+1) - 1      */
        idx_t den3 = den2 - 1;    /* 3(k+1) - 2      */

        comb = comb * (seq_t)num1 * (seq_t)num2 * (seq_t)num3
               / ((seq_t)den1 * (seq_t)den2 * (seq_t)den3);
    }

    return sum;
}
