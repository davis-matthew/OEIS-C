/*
 * A356993  a(n) = b(n - b(n - b(n - b(n)))) for n >= 2, where b(n) =
 *          A356988(n).
 * https://oeis.org/A356993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A356993
 *
 *  a(n) = b(n - b(n - b(n - b(n))))   for n >= 2
 *  where b(n) is A356988.
 *  The sequence is "slow": a(n+1)-a(n) is 0 or 1.
 *
 *  This implementation uses the closed‑form plateau description
 *  provided in the OEIS entry.  For  n <= 5 the value is 1.
 *
 *  For k >= 3 the following formulas hold:
 *    a(3*F(k)+j)           = F(k)                      0 <= j <= F(k-1)
 *    a(L(k+1)+j)           = F(k)+j                    0 <= j <= F(k-2)
 *    a(4*F(k)+j)           = L(k-1)                    0 <= j <= F(k-1)
 *    a(4*F(k)+F(k-1)+j)    = L(k-1)+j                   0 <= j <= F(k-3)
 *  where F(k) are Fibonacci numbers (F(0)=0,F(1)=1) and
 *  L(k) are Lucas numbers (L(0)=2,L(1)=1).
 *
 *  Complexity:  O(k) with k ≈ log n.  For n up to 10 000 k<25.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n < 2)                    return (seq_t)0;   /* undefined value */
    if (n <= 5)                   return (seq_t)1;   /* a(2)…a(5)=1 */

    /* Convert to seq_t only once for comparisons */
    const seq_t sn = (seq_t)n;

    /* Fibonacci and Lucas tables (static to avoid repeated allocation) */
    enum { MAXK = 80 };            /* large enough for n <= 10^4; safe for bigger */
    static seq_t F[MAXK];
    static seq_t L[MAXK];
    static int initialized = 0;

    if (!initialized) {
        F[0] = 0;   F[1] = 1;
        for (int i = 2; i < MAXK; ++i) F[i] = F[i-1] + F[i-2];

        L[0] = 2;   L[1] = 1;
        for (int i = 2; i < MAXK; ++i) L[i] = L[i-1] + L[i-2];

        initialized = 1;
    }

    /* Loop over k starting at 3 (formulas are valid only for k >= 3) */
    for (int k = 3; k < MAXK; ++k) {
        /* 1. a(3*F(k)+j) = F(k)                     0 <= j <= F(k-1) */
        const seq_t start1 = (seq_t)3 * F[k];
        const seq_t end1   = start1 + F[k-1];
        if (sn >= start1 && sn <= end1)  return F[k];

        /* 2. a(L(k+1)+j) = F(k)+j                 0 <= j <= F(k-2) */
        const seq_t start2 = L[k+1];
        const seq_t end2   = start2 + F[k-2];
        if (sn >= start2 && sn <= end2)  return F[k] + (sn - start2);

        /* 3. a(4*F(k)+j) = L(k-1)                 0 <= j <= F(k-1) */
        const seq_t start3 = (seq_t)4 * F[k];
        const seq_t end3   = start3 + F[k-1];
        if (sn >= start3 && sn <= end3)  return L[k-1];

        /* 4. a(4*F(k)+F(k-1)+j) = L(k-1)+j        0 <= j <= F(k-3) */
        const seq_t start4 = start3 + F[k-1];
        const seq_t end4   = start4 + F[k-3];
        if (sn >= start4 && sn <= end4)  return L[k-1] + (sn - start4);
    }

    /* Should never be reached for defined n, return 0 as a safety fall‑back */
    return (seq_t)0;
}
