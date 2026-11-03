/*
 * A285779  Parity index: number of integers z with 1 <= z <= n for which
 *          A010060(z) = A010060(n), negated if A010060(n) = 1.
 * https://oeis.org/A285779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^{A010060(n)} * (number of integers z with 1 ≤ z ≤ n
 *            for which A010060(z)=A010060(n)). A010060(n) is the parity of
 *            popcount(n). Even parity yields a positive count; odd parity
 *            yields a negative count
 * Range:     i8 n <= 255, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9591 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* count numbers x with 0 ≤ x ≤ n having even popcount */
static seq_t even_count_upto(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* only 0, popcount 0 (even) */

    /* find floor(log2(n)) */
    unsigned lg = 0;
    idx_t tmp = n;
    while (tmp >>= 1) ++lg;

    idx_t k = (idx_t)1 << lg;              /* highest power of two ≤ n   */
    idx_t r = n - k;                       /* remainder after removing msb */

    /* even count for numbers < k (i.e., 0..k-1) */
    seq_t Ekminus1;
    if (k == 1)                          /* k = 1 → n = 0 case */
        Ekminus1 = (seq_t)1;
    else
        Ekminus1 = (seq_t)(k >> 1);      /* for k=2^p, E(k-1)=k/2 */

    seq_t Er = even_count_upto(r);        /* recursive part */

    return Ekminus1 + ((seq_t)(r + 1) - Er);
}

seq_t a(idx_t n)
{
    seq_t E = even_count_upto(n);              /* numbers ≤ n with even popcount */
    unsigned long long parity = __builtin_parityll((unsigned long long)n);
    /* parity == 0 → even popcount */

    if (parity == 0) {                        /* even popcount ⇒ positive */
        return E - 1;                         /* exclude 0 */
    } else {                                  /* odd popcount ⇒ negative */
        seq_t tot = (seq_t)n + 1;            /* n+1 values 0..n           */
        seq_t oddCount = tot - E;            /* numbers with odd popcount  */
        return -oddCount;
    }
}
