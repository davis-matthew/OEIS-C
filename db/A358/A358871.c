/*
 * A358871  Square array A(n, k), n, k >= 0, read by antidiagonals: A(0, 0) =
 *          0, A(0, 1) = A(1, 0) = 1, A(1, 1) = 2, for n, k >= 0, A(2*n, 2*k)
 *          = A(n, k), A(2*n, 2*k+1) = A(n, k) + A(n, k+1), A(2*n+1, 2*k) =
 *          A(n, k) + A(n+1, k), A(2*n+1, 2*k+1) = A(n+1, k+(1+(-1)^(n+k))/2)
 *          + A(n, k+(1-(-1)^(n+k))/2).
 * https://oeis.org/A358871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the square array A filled by the
 *            recurrences below, read along the antidiagonals. Array A(n,k)
 *            (n,k ≥ 0): A(0,0)=0, A(0,1)=A(1,0)=1, A(1,1)=2 For n,k ≥ 0:
 *            A(2n,2k) = A(n,k) A(2n,2k+1) = A(n,k) + A(n,k+1) A(2n+1,2k) =
 *            A(n,k) + A(n+1,k) A(2n+1,2k+1)=A(n+1, k + (1+(-1)^(n+k))/2)...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---- Stern‑diatomic sequence A002487(n) ---- */
static seq_t stern(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if ((n & 1ULL) == 0) return stern(n >> 1);
    return stern(n >> 1) + stern((n >> 1) + 1);
}

/* ---- Recursive evaluation of A(n,k) following the rules above ---- */
static seq_t A_rec(idx_t n, idx_t k)
{
    /* Symmetry: A(n,k) = A(k,n) – keep the pair ordered for easier
       recursion but allow it inside the function (it will swap again). */
    if (n < k) { idx_t t = n; n = k; k = t; }

    /* Base cases. */
    if (n == 0 && k == 0) return (seq_t)0;
    if (k == 0)           return stern(n); /* A(n,0)  */
    if (n == 0)           return stern(k); /* A(0,k)  */
    if (n == 1 && k == 1) return (seq_t)2; /* A(1,1)  */

    /* Recursive cases. */
    if ((n & 1ULL) == 0 && (k & 1ULL) == 0)  /* even, even */
        return A_rec(n >> 1, k >> 1);

    if ((n & 1ULL) == 0 && (k & 1ULL) == 1)  /* even, odd  */
        return A_rec(n >> 1, k >> 1) +
               A_rec(n >> 1, (k >> 1) + 1);

    if ((n & 1ULL) == 1 && (k & 1ULL) == 0)  /* odd, even  */
        return A_rec(n >> 1, k >> 1) +
               A_rec((n >> 1) + 1, k >> 1);

    /* odd, odd  */
    idx_t m = n >> 1;   /* m = floor(n/2)  */
    idx_t p = k >> 1;   /* p = floor(k/2)  */
    if (((m + p) & 1ULL) == 0) /* (m+p) even -> k+p+(1+...)/2 = p+1, (1-...)/2 = 0 */
        return A_rec(m + 1, p + 1) + A_rec(m, p);
    else                     /* (m+p) odd  */
        return A_rec(m + 1, p) + A_rec(m, p + 1);
}

/* ---- public entry point: a(n) ---- */
seq_t a(idx_t n)
{
    /* Find antidiagonal index t such that t*(t+1)/2 ≤ n < (t+1)*(t+2)/2. */
    idx_t t = 0;
    while (((t + 1) * (t + 2)) / 2 <= n) ++t;

    idx_t first_on_diag = t * (t + 1) / 2;
    idx_t offset = n - first_on_diag;   /* position on the diagonal, 0‑based */
    idx_t i = offset;                   /* row index */
    idx_t j = t - offset;               /* column index */

    return A_rec(i, j);
}
