/*
 * A250726  Number of (n+1) X (5+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in * G(x) =
 *            x*(257-946x+1180x^2-110x^3-1079x^4+1060x^5-440x^6+93x^7-12x^8+x^9)
 *            /* --------------------------------------------- * (1-x)^7*(1+x)
 *            * Implementation uses the alternating binomial sum *
 *            S(m)=∑_{t=0}^m (-1)^t C(m-t+6,6) = (-1)^m∑_{u=0}^m (-1)^u
 *            C(u+6,6). *...
 * Range:     u8 none, u16 n <= 11, u32 n <= 102, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* coefficients of the numerator polynomial, indexed by k where the term is
 * b_k * x^(k+1).  The sequence uses these 10 coefficients. */
static const int64_t num_coeff[10] = {
    257,   /* x^1   */
   -946,   /* x^2   */
   1180,   /* x^3   */
   -110,   /* x^4   */
  -1079,   /* x^5   */
   1060,   /* x^6   */
   -440,   /* x^7   */
     93,   /* x^8   */
   -12,   /* x^9   */
     1    /* x^10  */
};

/* cached binomial and alternating-sum arrays */
static unsigned __int128 *C_arr  = NULL;   /* C(m+6,6)  */
static __int128        *T_arr  = NULL;   /* T(m)=∑_{u=0}^m (-1)^u C(u+6,6) */
static size_t  cache_size = 0;           /* number of entries pre‑computed (≥1) */

/* resize the cache to at least 'needed' entries (indexes 0…needed-1) */
static void ensure_cache(idx_t needed_idx)
{
    size_t needed = (size_t)needed_idx;
    if (needed < 1) needed = 1;               /* at least index 0 */
    if (needed <= cache_size) return;

    unsigned __int128 *newC = (unsigned __int128*)malloc(needed * sizeof(unsigned __int128));
    __int128        *newT = (__int128*)malloc(needed * sizeof(__int128));

    for (size_t i = 0; i < needed; ++i) {
        if (i == 0) {
            newC[0] = 1;
            newT[0] = 1;
        } else {
            /* C(i+6,6) = C(i-1+6,6) * (i+6) / i  */
            newC[i] = newC[i-1] * ((unsigned __int128)(i + 6)) / (unsigned __int128)i;

            int sign = ( (i & 1) == 0 ) ? 1 : -1;
            if (sign > 0)
                newT[i] = newT[i-1] + ( (__int128) newC[i] );
            else
                newT[i] = newT[i-1] - ( (__int128) newC[i] );
        }
    }

    free(C_arr);
    free(T_arr);
    C_arr  = newC;
    T_arr  = newT;
    cache_size = needed;
}

/* the required exported function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* invocations below offset are irrelevant */

    /* need binomial values up to m = n-1 (for k=0) */
    ensure_cache((idx_t)n);

    __int128 sum = 0;

    /* number of terms that contribute */
    size_t max_k = (n > 10) ? 10 : (size_t)n;  /* indices 0..max_k-1 */

    for (size_t k = 0; k < max_k; ++k) {
        size_t m = (size_t)n - (k + 1);      /* m = n-(k+1) >= 0 */
        /* S(m) = (-1)^m * T(m)  */
        int sign_m = ( (m & 1) == 0 ) ? 1 : -1;
        __int128 contrib = (__int128)num_coeff[k] * T_arr[m] * ( (__int128) sign_m );
        sum += contrib;
    }

    /* the sequence values are non‑negative */
    return (seq_t)sum;
}
