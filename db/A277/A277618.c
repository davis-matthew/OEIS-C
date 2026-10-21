/*
 * A277618  Lexicographically earliest nonnegative sequence such that
 *          |a(n+1)-a(n)| is a prime number, and no number occurs twice; a(0)
 *          = 0.
 * https://oeis.org/A277618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = a(n-5) + 5  for all n > 5 (with a(0)=0, a(1)=2, a(2)=4, a(3)=1, a(4)=3) */
/*  Equivalent closed form: a(n) = base[n%5] + 5*(n/5)  where */
/*  base = {0,2,4,1,3}.  This holds for all n ≥ 0. */

seq_t a(idx_t n)
{
    /* Base values for the first five indices. */
    static const seq_t base[5] = {(seq_t)0, (seq_t)2, (seq_t)4, (seq_t)1, (seq_t)3};

    /* Compute quotient and remainder modulo 5. */
    idx_t q = n / 5;
    idx_t r = n % 5;

    /* Result in type seq_t. */
    return base[r] + (seq_t)5 * (seq_t)q;
}
