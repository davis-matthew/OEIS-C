/*
 * A357743  Square array A(n, k), n, k >= 0, read by antidiagonals: A(0, 0) =
 *          0, A(0, 1) = A(1, 0) = 1, for n, k >= 0, A(2*n, 2*k) = A(n, k),
 *          A(2*n, 2*k+1) = A(n, k) + A(n, k+1), A(2*n+1, 2*k) = A(n, k) +
 *          A(n+1, k), A(2*n+1, 2*k+1) = A(n, k+1) + A(n+1, k).
 * https://oeis.org/A357743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(n,0) where A(n,k) is defined recursively in the OEIS
 *            description
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Stern's diatomic sequence: a(0)=0, a(1)=1,
 * a(2n)=a(n), a(2n+1)=a(n)+a(n+1).  This is exactly A(n,0). */
static seq_t stern(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;            /* missing base case that caused a crash */
    if ((n & 1) == 0)                 /* even */
        return stern(n >> 1);
    else                               /* odd */
    {
        idx_t n2 = n >> 1;
        return stern(n2) + stern(n2 + 1);
    }
}

/* Recursive evaluation of the array A(n,k) using the recurrence
 * given in the OEIS description.  Symmetry A(n,k)=A(k,n) is used
 * to keep indices small. */
static seq_t A_value(idx_t n, idx_t k)
{
    if (n == 0 && k == 0) return 0;
    if (k == 0) return stern(n);          /* n>=1 */
    if (n == 0) return stern(k);          /* symmetry for k>0 */
    if (n < k) return A_value(k, n);      /* ensure n>=k */

    /* n>=1 and k>=1 */
    if ((n & 1) == 0) {                   /* n even: n = 2*n2 */
        idx_t n2 = n >> 1;
        if ((k & 1) == 0) {               /* k even: k = 2*k2 */
            idx_t k2 = k >> 1;
            return A_value(n2, k2);
        } else {                          /* k odd  : k = 2*k2+1 */
            idx_t k2 = k >> 1;
            seq_t a1 = A_value(n2, k2);
            seq_t a2 = A_value(n2, k2 + 1);
            return a1 + a2;
        }
    } else {                               /* n odd: n = 2*n2+1 */
        idx_t n2 = n >> 1;
        if ((k & 1) == 0) {               /* k even: k = 2*k2 */
            idx_t k2 = k >> 1;
            seq_t a1 = A_value(n2, k2);
            seq_t a2 = A_value(n2 + 1, k2);
            return a1 + a2;
        } else {                          /* k odd: k = 2*k2+1 */
            idx_t k2 = k >> 1;
            seq_t a1 = A_value(n2, k2 + 1);
            seq_t a2 = A_value(n2 + 1, k2);
            return a1 + a2;
        }
    }
}

/* Map the global index n onto the (i,j) coordinates of the square array
 * and return A(i,j).  The mapping uses antidiagonals of the array. */
seq_t a(idx_t n)
{
    /* Find the diagonal sum 's' such that the first element of
     * that diagonal is at index < n and the last element is at
     * index >= n.  The number of elements up to diagonal s is
     * (s+1)(s+2)/2. */
    idx_t s = 0;
    idx_t cum = 1;                       /* count of elements up to sum 0 */
    while (cum <= n) {
        ++s;
        cum += s + 1;                    /* add elements of the next diagonal */
    }
    /* Elements before diagonal s:  s*(s+1)/2  */
    idx_t prev = (s * (s + 1)) / 2;
    idx_t offset = n - prev;             /* position inside diagonal (0..s) */
    idx_t i = offset;                    /* row index  */
    idx_t j = s - offset;                /* column index */
    return A_value(i, j);
}
