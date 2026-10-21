/*
 * A284130  Hosoya triangle of Jacobsthal type, read by rows.
 * https://oeis.org/A284130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the Jacobsthal‑type Hosoya triangle
 *            * row by row. T(n,k)=J(k)*J(n-k+1) where J(0)=0, J(1)=1 and *
 *            J(m)=J(m-1)+2*J(m-2) (Jacobian numbers). The sequence a(n) is
 *            the * concatenation of all rows of the triangle: a(1)=1, a(2)=1,
 *            a(3)=1, * a(4)=3, a(5)=1, a(6)=3, …
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 2145,
 *            u128 n <= 8385, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
static seq_t *jacob = NULL;    /* array of Jacobsthal numbers, indices 0..m */
static size_t  jlen = 0;       /* allocated size (count) */
 
/* Ensure that J[0..need] are available. */
static void ensure_jacob(idx_t need)
{
    if ((idx_t)jlen > need) return;        /* already computed */
    size_t newlen = (size_t)need + 1;
    seq_t *tmp = (seq_t *)realloc(jacob, newlen * sizeof(seq_t));
    if (!tmp) { /* allocation failure – let caller handle via abort() if desired */ }
    jacob = tmp;
    /* Initialize beginning if this is the first allocation */
    if (jlen == 0) {
        jacob[0] = (seq_t)0;       /* J(0) */
        if (newlen > 1) jacob[1] = (seq_t)1; /* J(1) */
        jlen = (newlen > 1) ? newlen : 2;
    }
    /* Compute missing values up to 'need' */
    for (size_t i = jlen; i <= (size_t)need; ++i) {
        jacob[i] = jacob[i-1] + ((seq_t)2 * jacob[i-2]);
    }
    jlen = (size_t)need + 1;
}
 
/* Find the smallest r such that r*(r+1)/2 >= n */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
 
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n==0, but safe */
    idx_t r = find_row(n);                                /* row index */
    idx_t t_prev = (r - 1) * r / 2;                        /* triangular number of previous row */
    idx_t k = n - t_prev;                                  /* position within row (1 ≤ k ≤ r) */
    /* Ensure Jacobsthal numbers up to r are computed */
    if ((idx_t)jlen <= r) ensure_jacob(r);
    /* Compute the term */
    seq_t left  = jacob[(size_t)k];
    seq_t right = jacob[(size_t)(r - k + 1)];
    return left * right;
}
