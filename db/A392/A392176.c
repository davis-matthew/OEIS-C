/*
 * A392176  Array A(n,k) = (n*k-1)*(n*k-2)/2 (n>=1, k>=1) read by
 *          antidiagonals.
 * https://oeis.org/A392176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the nth term of OEIS A392176, where the array A(i,j)
 *            = (i*j-1)*(i*j-2)/2 is read by antidiagonals
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=3, a(6)=1, ... */

seq_t a(idx_t n)
{
    /* Parameters are 1-indexed; the sequence is defined for n >= 1. */
    if (n == 0) {
        return 0;            /* behavior below the offset is irrelevant */
    }

    /* Find the diagonal d such that d*(d+1)/2 >= n
       and (#terms up to previous diagonal) < n. */
    idx_t d = 1;
    idx_t tri = 1;            /* d*(d+1)/2 for current d */

    /* For n up to about 10^6 or larger, this loop is trivial
       (d is O(sqrt(n))). */
    while (tri < n) {
        ++d;
        tri += d;             /* tri becomes d*(d+1)/2 progressively */
    }

    /* Number of terms up to previous diagonal */
    idx_t prevTri = tri - d;
    /* Position within current diagonal (1-based) */
    idx_t p = n - prevTri;

    /* Coordinates (i,j) in the array */
    idx_t i = p;
    idx_t j = d + 1 - p;

    /* Compute the array value A(i,j) = (i*j-1)*(i*j-2)/2
       Use seq_t arithmetic and guard against underflow for small i*j. */
    seq_t nk = (seq_t)i * (seq_t)j;

    if (nk <= 2) {
        return 0;            /* for (1,1) and (1,2)/(2,1) the value is 0 */
    } else {
        return (nk - 1) * (nk - 2) / 2;
    }
}
