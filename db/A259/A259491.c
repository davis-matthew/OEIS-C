/*
 * A259491  Expansion of (eta(q)^2 * eta(q^2) * eta(q^4)^3 / eta(q^8)^2)^2 in
 *          powers of q.
 * https://oeis.org/A259491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of (eta(q)^2 * eta(q^2) * eta(q^4)^3 / eta(q^8)^2)^2 *
 *            in powers of q. This is equivalent to the product * * ∏_{k≥1}
 *            ((1 - q^k)^4 (1 + q^k)^2 (1 + q^(2k)) / (1 + q^(4k))^2)^2 * *
 *            which can be rewritten as * * ∏_{n≥1} (1 - q^n)^{e_n} * with * *
 *            e_n = 4 + (n even ? 2 : 0) + (n ≡ 0 mod 4 ?...
 * Range:     i8 n <= 6, i16 n <= 19, i32 n <= 338, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *seq = NULL;
    static idx_t size = 0;
    /* if we already know enough terms, return quickly */
    if (n < size)
        return seq[n];

    /* Allocate fresh arrays for computation up to n */
    idx_t m, k;
    seq_t *new_seq = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (new_seq == NULL) return 0;          /* silent failure */
    seq_t *e = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *E = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (e == NULL || E == NULL) {
        free(new_seq);
        free(e);
        free(E);
        return 0;
    }

    /* compute the exponents e_d for 1 ≤ d ≤ n */
    for (m = 1; m <= n; ++m) {
        if (m & 1U) {          /* odd */
            e[m] = 4;
        } else {               /* even */
            seq_t val = 4 + 2; /* even adds 2 */
            if ((m & 3U) == 0U)   val += 6;   /* divisible by 4 */
            if ((m & 7U) == 0U)   val -= 4;   /* divisible by 8 */
            e[m] = val;
        }
    }

    /* compute divisor sums S(k) = Σ_{d|k} e_d · d */
    for (m = 1; m <= n; ++m) {
        seq_t val = e[m] * (seq_t)m;
        for (k = m; k <= n; k += m)
            E[k] += val;
    }

    /* recurrence for a(n) */
    new_seq[0] = 1;
    for (m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (k = 1; k <= m; ++k)
            sum += E[k] * new_seq[m - k];
        new_seq[m] = -(sum / (seq_t)m);
    }

    /* free old cache */
    free(seq);
    free(e);
    free(E);
    seq   = new_seq;
    size  = n + 1;
    return seq[n];
}
