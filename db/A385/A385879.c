/*
 * A385879  Values of u in triples (u, v, w) such that the polynomial x^3 +
 *          u*x^2 + v*x + w has 3 (possibly repeated) negative integer zeros;
 *          the triples are ordered by the inequality u < v.
 * https://oeis.org/A385879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal u such that the number of unordered triples
 *            (a,b,c) of positive integers with a+b+c = u and a <= b <= c is
 *            at least n. The count of triples for sum u is p_3(u) =
 *            floor((u^2 + 3)/12)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A385879 sequence. */
seq_t a(idx_t n)
{
    /* n is 1-based offset.  For n=0 the behavior is irrelevant. */
    if (n == 0)
        return 0;

    idx_t s = 3;                     /* start from the smallest sum */
    seq_t cumulative = 0;            /* cumulative count of triples */

    /* iterate sums until the cumulative count reaches n */
    while (1) {
        seq_t sq = (seq_t)s * (seq_t)s;          /* s^2, use seq_t to be safe */
        seq_t triples = (sq + 3) / 12;           /* floor((s^2+3)/12) */
        cumulative += triples;

        if (cumulative >= (seq_t)n)            /* found the proper u */
            return (seq_t)s;

        s++;
    }
}
