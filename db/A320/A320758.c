/*
 * A320758  Number of ordered set partitions of [n] where the maximal block
 *          size equals two.
 * https://oeis.org/A320758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) - n! where f(0)=1, f(1)=1 and f(n) = n * f(n‑1) +
 *            C(n,2) * f(n‑2) counts ordered set partitions of [n] into blocks
 *            of size at most two
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* undefined for n<2, kept safe */
    seq_t f0 = 1;                   /* f(0) */
    seq_t f1 = 1;                   /* f(1) */
    seq_t fact = 1;                 /* 0! = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t comb = (seq_t)i * (seq_t)(i - 1);
        comb /= 2;                  /* C(i,2) */
        seq_t fi = (seq_t)i * f1 + comb * f0;
        f0 = f1;
        f1 = fi;
        fact = fact * (seq_t)i;     /* n! */
    }
    return f1 - fact;
}
