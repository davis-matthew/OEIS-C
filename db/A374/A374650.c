/*
 * A374650  a(n) = n! - 2(n-1)! + 3(n-2)! - 4(n-3)!.
 * https://oeis.org/A374650
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 2(n-1)! + 3(n-2)! - 4(n-3)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) is the first term) */

seq_t a(idx_t n)
{
    /* For n < 3 the behaviour is not defined in OEIS, return 0. */
    if (n < 3) {
        return 0;
    }

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t fact_n1 = fact / (seq_t)n;           /* (n-1)! */
    seq_t fact_n2 = fact_n1 / (seq_t)(n - 1);  /* (n-2)! */
    seq_t fact_n3 = fact_n2 / (seq_t)(n - 2);  /* (n-3)! */

    return fact                      /* n!   */
         - ((seq_t)2 * fact_n1)      /* -2(n-1)! */
         + ((seq_t)3 * fact_n2)      /* +3(n-2)! */
         - ((seq_t)4 * fact_n3);     /* -4(n-3)! */
}
