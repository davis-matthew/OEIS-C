/*
 * A286334  The smallest number a such that there exists an integer b such
 *          that a/b is equal to n% rounded to the nearest percent.
 * https://oeis.org/A286334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A286334: a(n) is the smallest positive integer a
   such that there exists an integer b where the percentage a/b, rounded to
   the nearest percent, equals n.  For n = 0 the value is 0. */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return (seq_t)0;

    /* For n > 0; use signed arithmetic for the small constants */
    seq_t two_n_minus_1 = (seq_t)2 * n - (seq_t)1;
    seq_t two_n_plus_1  = (seq_t)2 * n + (seq_t)1;

    for (seq_t a_val = 1;; ++a_val)
    {
        /* Compute the range of possible denominators b:
           ceil(200*a / (2n+1)) <= b <= floor(200*a / (2n-1))  */
        seq_t low_b  = ((seq_t)200 * a_val) / two_n_plus_1 + (seq_t)1;
        seq_t high_b = ((seq_t)200 * a_val) / two_n_minus_1;

        if (low_b <= high_b)
            return a_val;
    }
}
