/*
 * A303565  a(n) = [x^n] (Sum_{k=0..n} k!*x^k)/(Sum_{k=0..n} k!*(-x)^k).
 * https://oeis.org/A303565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A303565
   Formula: a(n) = [x^n] (Sum_{k=0..n} k!*x^k)/(Sum_{k=0..n} k!*(-x)^k)
   Recurrence: a(0) = 1
               a(n) = n! - Σ_{k=1}^{n} (-1)^k * k! * a(n-k)
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic memory
       management of arbitrary-precision integers. */
    static std::vector<seq_t> fact(1, (seq_t)1);  /* fact[0] = 1 */
    static std::vector<seq_t> val(1, (seq_t)1);   /* val[0]  = 1 */
    static idx_t max_n = 0;                       /* largest n already computed */

    if (n > max_n) {
        /* grow factorial vector */
        fact.resize(n + 1);
        for (idx_t i = max_n + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        /* grow value vector */
        val.resize(n + 1);
        for (idx_t m = max_n + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t prod = fact[k] * val[m - k];
                if (k & 1)
                    sum -= prod;    /* odd k: subtract */
                else
                    sum += prod;    /* even k: add    */
            }
            val[m] = fact[m] - sum;
        }
        max_n = n;
    }
    return val[n];
#else
    /* C implementation using malloc/realloc.  Works for any seq_t
       that supports the usual arithmetic operators. */
    static seq_t *fact = NULL;
    static seq_t *val  = NULL;
    static idx_t  max_n = 0;      /* largest n already computed */
    static size_t cap   = 0;      /* current capacity of arrays */

    /* allocate or grow arrays if necessary */
    if (cap == 0) {
        /* first call: allocate capacity for n+1 elements */
        cap = (size_t)n + 1;
        fact = (seq_t*)malloc(cap * sizeof(seq_t));
        val  = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!fact || !val) return 0;           /* allocation failure */
        fact[0] = (seq_t)1;
        val[0]  = (seq_t)1;
    } else if ((size_t)n + 1 > cap) {
        /* reallocate to a larger size */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        seq_t *new_val  = (seq_t*)realloc(val,  new_cap * sizeof(seq_t));
        if (!new_fact || !new_val) return 0;   /* allocation failure */
        fact = new_fact;
        val  = new_val;
        cap  = new_cap;
    }

    /* compute new factorials and a(n) values */
    for (idx_t i = max_n + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t prod = fact[k] * val[m - k];
            if (k & 1)
                sum -= prod;    /* odd k: subtract */
            else
                sum += prod;    /* even k: add    */
        }
        val[m] = fact[m] - sum;
    }

    max_n = n;
    return val[n];
#endif
}
