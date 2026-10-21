/*
 * A260163  Expansion of f(x^2)^2 / f(-x) in powers of x where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A260163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 140, u64 n <= 490,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1;
 *   for n>=1  a(n) = (1/n) * Σ_{k=1..n} d_k * a(n-k)
 *   with d_k = - Σ_{d|k} d * c_d,  c_d defined by
 *   c_d =  1  if d ≡ 0 (8)
 *   c_d = -1  if d ≡ 1,3,5,7 (8)
 *   c_d = -3  if d ≡ 2,6 (8)
 *   c_d =  3  if d ≡ 4 (8)
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
using bigint_t = std::conditional_t<
    std::is_integral<seq_t>::value,
    unsigned __int128,
    seq_t
>;
#else
typedef unsigned __int128 bigint_t;
#endif

static seq_t *A = NULL;          /* a(i) for i=0..max_n          */
static long long *D = NULL;      /* d_k for k=1..max_n            */
static idx_t max_n = 0;          /* largest n for which arrays are valid */

/* Compute d_k for 1..needed, then a(0..needed) */
static void recompute(idx_t needed)
{
    size_t sz = (size_t)needed + 1;
    seq_t *newA = (seq_t *)realloc(A, sz * sizeof(seq_t));
    long long *newD = (long long *)realloc(D, sz * sizeof(long long));
    if (!newA || !newD) abort();          /* memory allocation failed */

    A = newA;
    D = newD;

    /* pattern of the exponents c_d for d mod 8 */
    static const int pat[8] = {1, -1, -3, -1, 3, -1, -3, -1};

    /* initialise D[1..needed] to zero */
    for (idx_t i = 1; i <= needed; ++i) D[i] = 0;

    /* build d_k by adding contributions of every divisor d */
    for (idx_t d = 1; d <= needed; ++d) {
        int mod = (int)(d % 8);
        long long add = -(long long)d * (long long)pat[mod];
        for (idx_t m = d; m <= needed; m += d) {
            D[m] += add;
        }
    }

    /* recurrence for a(n) */
    A[0] = 1;
    for (idx_t n = 1; n <= needed; ++n) {
        bigint_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            long long dk = D[k];
            if (dk == 0) continue;
            unsigned long long absd = dk > 0 ? (unsigned long long)dk : (unsigned long long)(-dk);
            bigint_t prod = (bigint_t)(A[n - k]) * (bigint_t)absd;
            if (dk > 0) sum += prod;
            else       sum -= prod;
        }
        seq_t an = (seq_t)(sum / (bigint_t)n);
        A[n] = an;
    }
    max_n = needed;
}

/* Public entry point required by the problem statement */
seq_t a(idx_t n)
{
    if (A == NULL) recompute(0);
    if (n > max_n) recompute(n);
    return A[n];
}
