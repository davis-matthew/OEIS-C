/*
 * A257889  Terms satisfy: a(2*n) = a(n)*b(n) and a(2*n+1) = a(n)*b(n+1) for
 *          n>=0 with a(0)=1, where A(x)^2 = Sum_{n>=0} b(n)*x^n and g.f. A(x)
 *          = Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A257889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1 and for n≥1 a(2n) = a(n) * b(n) a(2n+1) = a(n) *
 *            b(n+1) where b(k) = ∑_{i=0}^{k} a(i) * a(k-i) (coefficients of
 *            A(x)^2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Make sure the arrays are large enough; new slots are zero‑initialised */
static void ensure_cap(idx_t need, seq_t **arr, size_t *cap)
{
    size_t sz = (size_t)need + 1;
    if (sz <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 1 : *cap;
    while (newcap < sz)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    /* zero the newly allocated part */
    memset(tmp + *cap, 0, (newcap - *cap) * sizeof(seq_t));
    *arr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL, *B = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static idx_t cur_max = 0;          /* highest index with a[ ] filled */

    /* Prepare storage */
    ensure_cap(n, &A, &a_cap);
    ensure_cap(n, &B, &b_cap);

    /* Initialise the very first terms once */
    if (cur_max == 0) {
        A[0] = (seq_t)1;
        B[0] = A[0] * A[0];          /* b(0) = 1 */
        if (n >= 1) {
            A[1] = (seq_t)1;
            /* b(1) = a(0)a(1)+a(1)a(0) = 2 */
            B[1] = A[0] * A[1] + A[1] * A[0];
            cur_max = 1;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        idx_t k = i >> 1;                      /* floor(i/2) */
        seq_t ai;
        if ((i & 1) == 0) /* even */
            ai = A[k] * B[k];
        else              /* odd */
            ai = A[k] * B[k + 1];
        A[i] = ai;

        /* Update b(i) = Σ_{j=0}^{i} a(j)*a(i-j) */
        seq_t bi = 0;
        for (idx_t j = 0; j <= i; ++j)
            bi += A[j] * A[i - j];
        B[i] = bi;
    }
    cur_max = n;

    return A[n];
}
