/*
 * A275643  Expansion of (1-q)^k/Product_{j=1..k} (1-q^j) for k=12.
 * https://oeis.org/A275643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (1-q)^12 / Π_{j=1..12} (1-q^j) a(n)
 *            = Σ_{i=0}^{min(12,n)} (-1)^i * C(12,i) * p_12(n-i) where p_12(m)
 *            is the number of partitions of m into parts ≤12
 * Range:     i8 n <= 1, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *part = NULL;          /* partition counts p_12[0..part_max]   */
static idx_t  part_max = 0;         /* largest m for which part[] is valid */
static seq_t  C[13];                /* binomial coefficients C(12,i) */
static int    C_init = 0;           /* flag whether C[] has been initialized */

static void init_binom(void)
{
    if (C_init) return;
    C[0] = 1;
    for (idx_t i = 1; i <= 12; ++i) {
        seq_t tmp = C[i-1] * (12 - (i-1));
        C[i] = tmp / i;  /* exact division */
    }
    C_init = 1;
}

/* Recompute partition array up to n (inclusive).  Overwrites previous data. */
static void recompute_part(idx_t n)
{
    if (part) free(part);
    part = (seq_t *)malloc((n+1) * sizeof(seq_t));
    if (!part) return;  /* allocation failure; caller will handle */

    for (idx_t i = 0; i <= n; ++i) part[i] = 0;
    part[0] = 1;  /* one way to partition 0 */

    for (idx_t j = 1; j <= 12; ++j) {
        for (idx_t m = j; m <= n; ++m) {
            part[m] += part[m - j];
        }
    }
    part_max = n;
}

seq_t a(idx_t n)
{
    init_binom();

    /* Ensure partition table covers required n */
    if (n >= part_max) recompute_part(n);
    if (!part) {          /* allocation failure */
        seq_t err = 0;    /* sentinel */
        return err;
    }

    seq_t result = 0;
    for (idx_t i = 0; i <= 12 && i <= n; ++i) {
        seq_t term = part[n - i] * C[i];
        if (i & 1) term = -term;   /* apply (-1)^i */
        result += term;
    }
    return result;
}
