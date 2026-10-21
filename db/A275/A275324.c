/*
 * A275324  Expansion of (x*(1-4*x^2)^(-3/2) + (1-4*x^2)^(-1/2) + x + 1)/2.
 * https://oeis.org/A275324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A275324
 *  a(0) = a(1) = 1
 *  For n ≥ 2:
 *      if n is even =  C(n, n/2) / 2
 *      if n is odd  =  n * C(n-1, (n-1)/2) / 2
 *
 *  C(n,k) is computed multiplicatively:
 *      C(n,k) = ∏_{i=1}^{k} (n-k+i)/i
 *      The division is exact at each step, so no overflow occurs in
 *      the intermediate product (for all standard sizes of seq_t).
 */

#include <stdbool.h>

/* ------------------------------------------------------------------- */
/*  Minimal binomial routine (name chosen to avoid clashing with
 *  pre‑existing helpers).                                    */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t res = 1;

    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i), then divide by i */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

/* ------------------------------------------------------------------- */
/*  Primary interface function                                         */
seq_t a(idx_t n)
{
    /* Offset 0: first term is a(0) = 1 */
    if (n == 0 || n == 1) return (seq_t)1;

    if ((n & 1ULL) == 0) {               /* n = 2k, k ≥ 1 */
        idx_t k = n >> 1;
        seq_t c = binom_idx(2 * k, k);   /* C(2k, k) */
        return c / 2;                    /* exact division */
    } else {                              /* n = 2k+1, k ≥ 1 */
        idx_t k = n >> 1;                /* (n-1)/2 */
        seq_t c = binom_idx(2 * k, k);   /* C(2k, k) */
        seq_t t = ((seq_t)n) * c;        /* n * C(2k, k) */
        return t / 2;                    /* exact division */
    }
}
