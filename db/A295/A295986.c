/*
 * A295986  Number of maximal cliques in the n-halved cube graph.
 * https://oeis.org/A295986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(n^2-5*n+12)*(n+2)/3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=1, a(3)=1 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= 3)
        return (seq_t)1;

    /* Compute 2^(n-4) by repeated doubling to avoid UB on large shifts */
    idx_t exp = n - 4;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;  /* wrapped multiplication on 64‑bit builds */

    /* Compute polynomial factors */
    seq_t nm = (seq_t)n;
    seq_t n_sq_minus5n_plus12 = nm * nm - (seq_t)5 * nm + (seq_t)12;
    seq_t n_plus2 = nm + (seq_t)2;

    /* Divide one factor by 3 to keep intermediate size modest */
    seq_t div_factor, other_factor;
    if ((n % 3u) == 1u) {          /* (n+2) divisible by 3 when n≡1 mod 3 */
        div_factor = n_plus2 / (seq_t)3;
        other_factor = n_sq_minus5n_plus12;
    } else {                       /* (n^2 - 5n + 12) divisible by 3 otherwise */
        div_factor = n_sq_minus5n_plus12 / (seq_t)3;
        other_factor = n_plus2;
    }

    /* Final multiplication – possible overflow on 64‑bit builds is acceptable */
    seq_t result = pow2 * div_factor;
    result = result * other_factor;

    return result;
}
