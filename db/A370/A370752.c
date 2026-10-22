/*
 * A370752  a(n) = 3^n * [x^n] Product_{k>=1} ((1 + 3*x^k)/(1 - 3*x^k))^(1/3).
 * https://oeis.org/A370752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n · [x^n] ∏_{k≥1} ((1+3x^k)/(1–3x^k))^(1/3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
typedef seq_t temp_t;
#else
typedef unsigned __int128 temp_t;          /* 128‑bit type for C (fractional) builds */
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;

    /* Allocate temporary arrays. */
    seq_t *pow3;
    seq_t *S;
    seq_t *a_arr;

#ifdef __cplusplus
    pow3 = new seq_t[N + 1];
    S    = new seq_t[N + 1];
    a_arr= new seq_t[N + 1];
#else
    pow3 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    S    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    a_arr= (seq_t *)malloc((N + 1) * sizeof(seq_t));
#endif
    if (!pow3 || !S || !a_arr)   /* should never happen */
        return (seq_t)0;

    /* Pre‑compute 3^i.  The values are below 2^64 for N <= 40. */
    pow3[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Pre‑compute the auxiliary sequence S[r] =
       2 · Σ_{d|r , r/d odd} d · 3^{r/d}.  */
    for (size_t r = 1; r <= N; ++r) {
        temp_t sum = 0;
        size_t root = (size_t)sqrt((double)r);
        for (size_t d = 1; d <= root; ++d) {
            if (r % d) continue;
            size_t k   = d;          /* divisor k in the product */
            size_t m   = r / d;      /* corresponding exponent */
            if (m & 1)                /* m odd? */
                sum += (temp_t)k * (temp_t)pow3[m] * (temp_t)2;
            if (m != k && (k & 1))    /* handle the complementary divisor */
                sum += (temp_t)m * (temp_t)pow3[k] * (temp_t)2;
        }
        S[r] = (seq_t)sum;
    }

    /* Compute the target sequence a_n.  The recurrence derived from the
       generating function is

           a_n = (1/n) · Σ_{r=1}^n 3^{r‑1} · S[r] · a_{n–r}.      */
    a_arr[0] = (seq_t)1;
    for (size_t cur = 1; cur <= N; ++cur) {
        temp_t sum = 0;
        for (size_t r = 1; r <= cur; ++r)
            sum += (temp_t)S[r] * (temp_t)a_arr[cur - r] * (temp_t)pow3[r - 1];
        sum /= (temp_t)cur;                 /* division is exact */
        a_arr[cur] = (seq_t)sum;
    }

    seq_t result = a_arr[N];

#ifdef __cplusplus
    delete[] pow3;
    delete[] S;
    delete[] a_arr;
#else
    free(pow3);
    free(S);
    free(a_arr);
#endif
    return result;
}
