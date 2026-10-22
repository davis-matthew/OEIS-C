/*
 * A307545  G.f. A(x) satisfies: A(x) = 1/(1 - x^a(0) - x^a(1) - x^a(2) -
 *          x^a(3) - ...) - x.
 * https://oeis.org/A307545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, and for n>=2, a(n) = Σ_{k: a(k)≤n} a(n-a(k)),
 *            i.e. a(n) is the number of compositions of n using parts
 *            a(0),a(1),…, with a(1) counted only once (hence a(1)=c(1)-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;  /* a(k) values */
    static seq_t *C = NULL;  /* composition counts c(k) */
    static idx_t size = 0;   /* number of entries initialized */
    static size_t cap  = 0;   /* allocated capacity */

    /* Ensure capacity */
    while (cap <= n) {
        size_t new_cap = cap ? cap << 1 : 8; /* double or start with 8 */
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        C = (seq_t*)realloc(C, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t m = size; m <= n; ++m) {
        if (m == 0) {                /* base case a(0) = 1 */
            A[0] = (seq_t)1;
            C[0] = (seq_t)1;
        } else if (m == 1) {         /* a(1) = 1, c(1) = 2 */
            seq_t sum = C[0] + C[0];  /* 1+1 = 2 */
            C[1] = sum;
            A[1] = sum - (seq_t)1;   /* 2-1 = 1 */
        } else {                     /* general case */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < size; ++k) {
                seq_t Ak = A[k];
                if ((idx_t)Ak > m) break;
                idx_t sub = m - (idx_t)Ak;
                sum += C[sub];
            }
            C[m] = sum;
            A[m] = sum;              /* a(m) = c(m) for m ≥ 2 */
        }
    }

    size = n + 1;   /* update number of computed terms */
    return A[n];
}
