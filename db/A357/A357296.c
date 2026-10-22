/*
 * A357296  Expansion of e.g.f. Sum_{k>0} x^k / (k! * (1 - x^k/k)).
 * https://oeis.org/A357296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{d|n} 1 / ( d^(n/d-1) * d! ) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_int.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* factorial cache (one‑time allocation) */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;   /* allocated capacity */
    static size_t fact_len = 0;   /* number of factorials stored (fact_len-1 is the largest index) */

    if (n < 1)  /* any behaviour below offset is irrelevant */
        return (seq_t)0;

    /* allocate and extend factorial array as necessary */
    if (fact_cap == 0) {
        fact_cap = 1;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (fact == NULL) return (seq_t)0;
        fact[0] = (seq_t)1;
        fact_len = 1;
    }
    while (fact_len <= n) {
        /* if capacity exceeded, grow the array */
        if (fact_len >= fact_cap) {
            size_t new_cap = fact_cap * 2;
            if (new_cap <= fact_len) new_cap = fact_len + 1;
            seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;
            fact = tmp;
            fact_cap = new_cap;
        }
        fact[fact_len] = fact[fact_len - 1] * (seq_t)fact_len;
        ++fact_len;
    }
    seq_t fact_n = fact[n];

    /* integer floor square root of n (no floating point) */
    idx_t sqrt_n = 0;
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * mid <= n) {
            sqrt_n = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    /* sum over all divisors of n */
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;               /* complementary divisor */

            /* term for divisor d */
            idx_t exp1 = d2 - 1;              /* n/d - 1 */
            seq_t denom1 = fact[d];
            denom1 *= pow_int((seq_t)d, exp1);
            sum += fact_n / denom1;

            /* term for complementary divisor d2, if distinct */
            if (d2 != d) {
                idx_t exp2 = d - 1;           /* n/d2 - 1 */
                seq_t denom2 = fact[d2];
                denom2 *= pow_int((seq_t)d2, exp2);
                sum += fact_n / denom2;
            }
        }
    }
    return sum;
}
