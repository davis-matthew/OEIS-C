/*
 * A330796  a(n) = Sum_{k=0..n} binomial(n, k)*(2^k - binomial(k,
 *          floor(k/2))).
 * https://oeis.org/A330796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k)*(2^k - binomial(k,
 *            floor(k/2)))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n) iterative sum, computing powers of 2
 * and central binomial coefficients incrementally, then iterating over
 * k to accumulate the result.  It works for any seq_t type (built‑in
 * integer or arbitrary‑precision) and is valid C11 and C++17. */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;

    size_t len = (size_t)n + 1;

#ifdef __cplusplus
    seq_t *pow2 = new seq_t[len];
    seq_t *central = new seq_t[len];
#else
    seq_t *pow2 = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *central = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!pow2 || !central) {
#ifdef __cplusplus
        delete[] pow2;
        delete[] central;
#else
        free(pow2);
        free(central);
#endif
        return (seq_t)0;
    }

    /* powers of 2 */
    pow2[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    /* central binomial coefficients: C(k, ⌊k/2⌋) */
    central[0] = (seq_t)1;
    if (n >= 1U)
        central[1] = (seq_t)1;
    for (size_t m = 1; 2 * m <= (size_t)n; ++m) {
        seq_t prev = central[2 * m - 2];
        seq_t mult = (seq_t)(4 * m - 2);
        seq_t div  = (seq_t)m;
        central[2 * m] = (prev * mult) / div;
    }
    for (size_t m = 1; 2 * m + 1 <= (size_t)n; ++m) {
        seq_t prev = central[2 * m - 1];
        seq_t mult = (seq_t)(4 * m + 2);
        seq_t div  = (seq_t)(m + 1);
        central[2 * m + 1] = (prev * mult) / div;
    }

    /* sum Σ C(n,k)*(pow2[k] - central[k]) */
    seq_t result = (seq_t)0;
    seq_t comb   = (seq_t)1;          /* C(n,0) */
    for (size_t k = 0; k < (size_t)n; ++k) {
        comb = (comb * ((seq_t)(n - k))) / (seq_t)(k + 1);
        seq_t diff = pow2[k + 1] - central[k + 1];
        result += comb * diff;
    }

#ifdef __cplusplus
    delete[] pow2;
    delete[] central;
#else
    free(pow2);
    free(central);
#endif
    return result;
}
