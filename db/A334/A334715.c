/*
 * A334715  A(n,k) = !n + [n > 0] * (k * n!), where !n = A000166(n) is
 *          subfactorial of n and [] is an Iverson bracket; square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A334715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A334715 – read the square array A(n,k)=!n + [n>0]*k*n! by
 *            antidiagonals
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 117, u64 n <= 251,
 *            u128 n <= 663, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the antidiagonal d that contains index n. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t start = d * (d + 1) / 2;   /* first index on diagonal d */
    idx_t t     = n - start;         /* offset within the diagonal */
    idx_t i     = t;                 /* first coordinate (n in A(n,k)) */
    idx_t j     = d - t;             /* second coordinate (k in A(n,k)) */

    /* Compute factorial(i) and subfactorial(i) */
    seq_t fact = 1;          /* i!   (fact[0] = 1)      */
    seq_t sub0 = 1;          /* !0    */
    seq_t sub1 = 0;          /* !1    */
    if (i == 0)
        return sub0;         /* a(0) = 1 */

    for (idx_t k = 1; k <= i; ++k) {
        fact *= (seq_t)k;            /* update factorial */
        if (k > 1) {                 /* update subfactorial */
            seq_t tmp = (seq_t)(k - 1) * (sub1 + sub0);
            sub0 = sub1;
            sub1 = tmp;
        }
    }

    /* A(i,j) = !i + j * i! for i > 0 */
    seq_t result = sub1;                      /* subfactorial(i) */
    result += (seq_t)j * fact;                /* add the k·i! part */
    return result;
}
