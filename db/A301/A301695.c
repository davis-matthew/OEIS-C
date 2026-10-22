/*
 * A301695  Expansion of (1 + 5*x + 4*x^2 + 5*x^3 + x^4)/((1 - x)^2*(1 -
 *          x^3)).
 * https://oeis.org/A301695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n*(n + 1) - 2*((n - 1)^2 mod 3) + 5)/3
 * Range:     u8 n <= 5, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Penalty: subtract 2 when (n-1)^2 mod 3 == 1 (i.e. n%3==0 or n%3==2), else 0. */
    idx_t mod3 = n % 3;
    seq_t penalty = (mod3 == 1) ? 0 : 2;   /* 2 * ((n - 1)^2 mod 3) */

    seq_t n_seq   = (seq_t)n;
    seq_t n_next  = n_seq + 1;
    seq_t num     = 8 * n_seq * n_next - penalty + 5;

    return num / 3;
}
