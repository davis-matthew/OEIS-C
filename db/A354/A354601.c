/*
 * A354601  Maximal GCD of ten positive integers with sum n.
 * https://oeis.org/A354601
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d | d divides n , d <= n/10 }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximal gcd of ten positive integers with sum n.
 * For n >= 10, the maximal gcd equals n divided by the smallest
 * divisor of n that is at least 10. */
seq_t a(idx_t n)
{
    /* For n < 10, the OEIS offset is not defined; return 0. */
    if (n < 10) {
        return 0;
    }

    /* Find the smallest divisor d of n with d >= 10. */
    idx_t min_div = n;                 /* initialise to n itself */
    for (idx_t i = 1; i <= n / i; ++i) /* iterate up to sqrt(n) safely */
    {
        if (n % i == 0) {
            /* i divides n */
            if (i >= 10 && i < min_div) {
                min_div = i;
            }

            idx_t j = n / i; /* complementary divisor */
            if (j >= 10 && j < min_div) {
                min_div = j;
            }
        }
    }

    /* The maximal gcd is n divided by that smallest divisor. */
    seq_t n_seq = (seq_t)n;
    return n_seq / (seq_t)min_div;
}
