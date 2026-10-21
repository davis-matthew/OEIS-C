/*
 * A260676  Expansion of phi(x) * psi(x^30) in powers of x where phi(), chi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A260676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1 n · a(n) = Σ_{k=1..n} c(k) · a(n‑k) where
 *            c(k) = Σ_{d|k} d · b(d) and b(d) = 2 + 2·I(4|d) + 1·I(30|d) –
 *            5·I(2|d) – 2·I(60|d) (I(·|·) is 1 if the divisor condition
 *            holds, otherwise 0). This is the Euler transform of the
 *            60‑periodic sequence b(k)
 * Range:     u8 n <= 86, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   periodic_b, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/periodic_b.h"

static seq_t *a_cache = NULL;          /* cache of computed a(n)   */
static size_t a_cap  = 20001;          /* initial cache capacity   */
static idx_t  computed_max = 0;        /* largest n already computed */
static int    c_cache[20001] = {0};    /* storage for c(k) values */

seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (a_cache == NULL) {
        a_cache = seq_alloc(a_cap);
        a_cache[0] = (seq_t)1;         /* a(0) = 1 */
    }

    /* expand the cache if n exceeds current capacity */
    if ((size_t)n >= a_cap) {
        size_t new_cap = n + 1;
        seq_t *new_arr = seq_alloc(new_cap);
        for (size_t i = 0; i < a_cap; ++i) new_arr[i] = a_cache[i];
        a_cache = new_arr;
        a_cap   = new_cap;
        /* we do not need to initialise beyond a_cap-1 yet */
    }

    /* compute missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* compute c(i) using divisors of i */
        int sumc = 0;
        for (idx_t d = 1; (idx_t)(d * d) <= i; ++d) {
            if (i % d == 0) {
                sumc += (int)d * periodic_b((int)d);
                idx_t other = i / d;
                if (other != d)
                    sumc += (int)other * periodic_b((int)other);
            }
        }
        c_cache[i] = sumc;

        /* recurrence: n*a(n) = Σ_k c(k)*a(n‑k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            int ck = c_cache[k];
            sum += (seq_t)ck * a_cache[i - k];
        }
        a_cache[i] = sum / ((seq_t)i);
    }
    computed_max = n;

    return a_cache[n];
}
