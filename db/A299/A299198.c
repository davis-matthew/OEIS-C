/*
 * A299198  a(n) = n^4/6 - 2*n^3/3 - n^2/6 + 5*n/3 + 1.
 * https://oeis.org/A299198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 401, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299198
   Formula: a(n) = (n^4 - 4*n^3 - n^2 + 10*n + 6) / 6
   (equivalent to (n-3)*(n+1)*(n^2 - 2*n - 2)/6)
   Base values (offset 1): a(1)=2, a(2)=1, a(3)=0, a(4)=5, a(5)=26. */

seq_t a(idx_t n)
{
    /* Handle the offset‐1 base terms explicitly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)26;

    /* For n >= 6 use the closed form. */
    seq_t nn = (seq_t)n;                   /* n as sequence type */

    /* Compute the factor (n^2 - 2*n - 2). */
    seq_t f3 = nn * nn - (seq_t)2 * nn - (seq_t)2;

    /* Compute (n+1)*(n^2 - 2*n - 2). */
    seq_t prod = (nn + (seq_t)1) * f3;

    /* Compute the final result: (n-3)*prod / 6. */
    if (sizeof(seq_t) <= 8) {
        /* For builtin small integer types use a wider unsigned
           intermediate to avoid overflow on intermediate products. */
        unsigned long long factor = (unsigned long long)(nn - (seq_t)3);
        unsigned long long res    = (unsigned long long)prod * factor;
        res /= 6ULL;
        return (seq_t)res;
    } else {
        /* For arbitrary‑precision types rely on the provided arithmetic. */
        seq_t factor = nn - (seq_t)3;
        seq_t res    = factor * prod;
        res /= (seq_t)6;
        return res;
    }
}
