/*
 * A371724  G.f. satisfies A(x) = ( 1 + x * A(x)^(1/3) * (1 + A(x)) )^(3/2).
 * https://oeis.org/A371724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371724: a(n) = coefficient of x^n in A(x),
   where A(x) = (1 + x*A(x)^(1/3)*(1 + A(x)))^(3/2).
   The sequence satisfies the algebraic equation
       x*Z^4 + x*Z + 1 = Z^2   with Z = A^(1/3).
   A(n) = coefficient of x^n in Z^3.
   This implementation builds the series coefficients for Z via
       s[n] = z[n-1] + t[n-1]          (recurrence for z^2)
       z[n] = (s[n] - sum_{i=1}^{n-1} z[i]*z[n-i]) / 2
   where s[n] = Σ_{i=0}^n z[i]*z[n-i]      (coefficients of Z^2)
         t[n] = Σ_{i=0}^n s[i]*s[n-i]      (coefficients of Z^4)
   Finally a[n] = Σ_{i=0}^n s[i]*z[n-i]  (coefficient of Z^3).
   Complexity: O(n^2) per maximum n, acceptable for n ≤ 10000.
*/

#include <stdlib.h>

/* static storage for the power‐series coefficients */
static seq_t *z = NULL;      /* coefficients of Z(x) */
static seq_t *s = NULL;      /* coefficients of Z^2 */
static seq_t *t = NULL;      /* coefficients of Z^4, i.e. convolution of s with itself */
static seq_t *a_arr = NULL;  /* final sequence A(n) = coefficients of Z^3 */
static size_t cap = 0;       /* current size of allocated arrays (number of terms) */

/* enlarge the arrays to hold coefficients up to index n (inclusive) */
static void ensure_terms(idx_t n) {
    if ((idx_t)cap > n) return;           /* already computed */

    size_t new_cap = (size_t)n + 1;        /* need 0..n inclusive */

    z      = (seq_t *)realloc(z,      new_cap * sizeof(seq_t));
    s      = (seq_t *)realloc(s,      new_cap * sizeof(seq_t));
    t      = (seq_t *)realloc(t,      new_cap * sizeof(seq_t));
    a_arr  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));
    if (!z || !s || !t || !a_arr) abort(); /* allocation failure */

    if (cap == 0) {                        /* first-time initialization */
        z[0] = (seq_t)1;
        s[0] = (seq_t)1;   /* 1*1 */
        t[0] = (seq_t)1;   /* 1*1 */
        a_arr[0] = (seq_t)1; /* 1^3 */
    }

    /* compute new terms from the previous capacity up to n */
    for (size_t k = cap; k <= (size_t)n; ++k) {
        if (k == 0) continue;  /* already known */

        /* sum_{i=1}^{k-1} z[i]*z[k-i]  (omit i=0 and i=k terms) */
        seq_t sum_z = 0;
        for (size_t i = 1; i < k; ++i) {
            sum_z += z[i] * z[k - i];
        }

        /* s[k] = coefficient of Z^2 at index k:  s[k] = z[k-1] + t[k-1] */
        s[k] = z[k - 1] + t[k - 1];

        /* z[k] = (s[k] - sum_z) / 2  (exact division) */
        z[k] = (s[k] - sum_z) / (seq_t)2;

        /* t[k] = convolution of s with itself:  t[k] = Σ_{i=0}^{k} s[i]*s[k-i] */
        seq_t sum_t = 0;
        for (size_t i = 0; i <= k; ++i) {
            sum_t += s[i] * s[k - i];
        }
        t[k] = sum_t;
    }

    /* compute the final sequence terms a[n] = coeff of Z^3 */
    for (size_t m = cap; m <= (size_t)n; ++m) {
        seq_t val = 0;
        for (size_t i = 0; i <= m; ++i) {
            val += s[i] * z[m - i];
        }
        a_arr[m] = val;
    }

    cap = (size_t)n + 1;
}

seq_t a(idx_t n) {
    ensure_terms(n);
    return a_arr[(size_t)n];
}
