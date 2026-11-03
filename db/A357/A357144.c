/*
 * A357144  Square array, A(n, k), n, k >= 0, read by antidiagonals; A(n, k) =
 *          g(f(n) * f(k)) where f(m) = A002487(m)/A002487(m+1) and g is the
 *          inverse of f.
 * https://oeis.org/A357144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(⌊k⌋, ⌊l⌋) where A(i,j) = g(f(i) * f(j)), f(m) =
 *            Stern(m)/Stern(m+1), g is the inverse of f. A002487(m) is
 *            Stern's diatomic sequence: a(0)=0, a(1)=1, a(2n)=a(n),
 *            a(2n+1)=a(n)+a(n+1). This implementation builds Stern up to the
 *            required indices, multiplies the corresponding fractions,
 *            reduc...
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

/* Inverse of f: given a reduced fraction num/den (num≥0,den>0), return the
   unique index m such that Stern(m)=num and Stern(m+1)=den.
   Special case: f(0)=0/1 corresponds to index 0. */
static idx_t cf_inverse(seq_t num, seq_t den)
{
    if (num == (seq_t)0) return 0;          /* f(0) = 0/1 → index 0 */
    /* The index is built from the path in the Stern–Brocot/Calkin–Wilf tree. */
    unsigned char bits[256]; /* enough for the depth of the tree */
    int top = 0;

    /* Traverse from the fraction up to 1/1, recording the direction of
       each step (0 = left child, 1 = right child).   */
    while (!(num == (seq_t)1 && den == (seq_t)1)) {
        if (num < den) {
            den -= num;      /* move to parent via left child */
            bits[top++] = 0;
        } else {            /* num > den */
            num -= den;      /* move to parent via right child */
            bits[top++] = 1;
        }
    }

    /* Reconstruct the index from the recorded bits, which are in reverse
       order (leaf→root).  The base of the tree has index 1. */
    idx_t m = 1;
    for (int i = top - 1; i >= 0; --i) {
        m = (m << 1) | bits[i];
    }
    return m;
}

/* Cache for Stern's diatomic sequence, 0‑based. */
static seq_t *stern_cache = NULL;  /* array of Stern values */
static idx_t   stern_size  = 0;     /* number of elements cached (exclusive) */
static idx_t   stern_cap   = 0;     /* allocated capacity */

/* Ensure that Stern values up to index m are cached (m inclusive). */
static void ensure_stern(idx_t m)
{
    if (stern_cache == NULL) {
        stern_cap = 16;
        stern_cache = mem_new(seq_t, stern_cap);
        stern_cache[0] = (seq_t)0;
        stern_cache[1] = (seq_t)1;
        stern_size   = 2;
    }

    if (m < stern_size) return;

    if ((idx_t)stern_cap < m + 1) {
        stern_cap = ((m + 2) * 2 > (idx_t)UINT32_MAX ? (idx_t)UINT32_MAX : (m + 2) * 2);
        stern_cache = mem_resize(seq_t, stern_cache, stern_cap);
    }

    for (idx_t i = stern_size; i <= m; ++i) {
        if ((i & 1) == 0)
            stern_cache[i] = stern_cache[i / 2];
        else
            stern_cache[i] = stern_cache[i / 2] + stern_cache[i / 2 + 1];
    }
    stern_size = m + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Rows or columns that contain a zero entry are handled immediately. */
    if (i == 0 || j == 0) return (seq_t)0;

    /* Ensure Stern values are available. */
    ensure_stern(i + 1);
    ensure_stern(j + 1);

    /* Retrieve fractions f(i) = S(i)/S(i+1), f(j) = S(j)/S(j+1). */
    seq_t num1 = stern_cache[i];
    seq_t den1 = stern_cache[i + 1];
    seq_t num2 = stern_cache[j];
    seq_t den2 = stern_cache[j + 1];

    /* Multiply the rationals: num/den = (num1*num2)/(den1*den2). */
    seq_t num = num1 * num2;
    seq_t den = den1 * den2;

    /* Reduce to lowest terms. */
    seq_t g = gcd(num, den);
    num /= g;
    den /= g;

    /* Map the reduced fraction back to the index using the inverse of f. */
    return (seq_t)cf_inverse(num, den);
}
