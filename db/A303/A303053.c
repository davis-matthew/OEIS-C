/*
 * A303053  Number of minimum total dominating sets in the n-prism graph.
 * https://oeis.org/A303053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 if n ≡ 0 (mod 6) a(n) = 2*n if n ≡ 1 (mod 6) a(n) = n*n
 *            if n ≡ 2 (mod 6) a(n) = 3 if n ≡ 3 (mod 6) a(n) = ((2*k +
 *            3)*(3*k + 2))^2 if n = 6*k + 4 a(n) = (4*k + 5)*(6*k + 5) if n =
 *            6*k + 5 where k = ⌊n/6⌋
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‐form formulas given in the OEIS entry,
   so it works for all n ≥ 1 exactly. */

seq_t a(idx_t n)
{
    /* n is 1‑based index (n ≥ 1) */
    idx_t r = n % 6;          /* remainder modulo 6 */
    switch (r) {
    case 0:  /* n = 6*k */
        return (seq_t)9;

    case 1:  /* n = 6*k + 1 */
        return (seq_t)2 * (seq_t)n;

    case 2:  /* n = 6*k + 2 */
        return (seq_t)n * (seq_t)n;

    case 3:  /* n = 6*k + 3 */
        return (seq_t)3;

    case 4:  /* n = 6*k + 4, k = floor(n/6) */
    {
        idx_t k = n / 6;
        seq_t t1 = (seq_t)(2 * k + 3);
        seq_t t2 = (seq_t)(3 * k + 2);
        seq_t prod = t1 * t2;
        return prod * prod;
    }

    case 5:  /* n = 6*k + 5, k = floor(n/6) */
    {
        idx_t k = n / 6;
        seq_t prod = (seq_t)(4 * k + 5) * (seq_t)(6 * k + 5);
        return prod;
    }

    default: /* unreachable, but keep compiler happy */
        return (seq_t)0;
    }
}
