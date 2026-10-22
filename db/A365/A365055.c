/*
 * A365055  E.g.f. satisfies A(x) = exp( x * (1+x/2) * A(x)^3 ).
 * https://oeis.org/A365055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (1/2)^(n-k) * (3*k+1)^(k-1) *
 *            binomial(k,n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> fact_vec;
#define FACT(n) (fact_vec[(size_t)(n)])
#else
static seq_t *fact_arr = NULL;
static size_t fact_cap = 0;
#define FACT(n) (fact_arr[(size_t)(n)])
#endif

static void ensure_factorial(idx_t n)
{
#ifdef __cplusplus
    while (fact_vec.size() <= (size_t)n) {
        size_t i = fact_vec.size();
        if (i == 0) {
            fact_vec.push_back((seq_t)1);
        } else {
            fact_vec.push_back(fact_vec.back() * (seq_t)i);
        }
    }
#else
    if (fact_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = realloc(fact_arr, new_cap * sizeof(seq_t));
        if (!tmp) return;                       /* allocation failed; 
                                                    caller may not use result */
        fact_arr = tmp;
        for (size_t i = fact_cap; i <= (size_t)n; ++i) {
            if (i == 0)
                fact_arr[i] = (seq_t)1;
            else
                fact_arr[i] = fact_arr[i-1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }
#endif
}

seq_t a(idx_t n)
{
    ensure_factorial(n);
    seq_t fact_n = FACT(n);
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(k, n - k);
        if (bin == 0) continue;

        /* term = (n! / k!) * binomial(k,n-k) * (3k+1)^(k-1) / 2^(n-k) */
        seq_t term = fact_n;
        term /= FACT(k);           /* now term = n! / k! */
        term *= bin;               /* multiply by binomial */
        if (k > 0) {
            seq_t pow_term = big_pow((seq_t)(3 * k + 1), (seq_t)(k - 1));
            term *= pow_term;
        }
        seq_t denom = pow2(n - k);  /* 2^(n-k) */
        term /= denom;
        sum += term;
    }
    return sum;
}
