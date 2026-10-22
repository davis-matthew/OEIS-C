/*
 * A301829  Number of ways to choose a nonempty submultiset of a factorization
 *          of n into factors greater than one.
 * https://oeis.org/A301829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d>1} f(d) * f(n/d) * where f(n) = A001055(n) –
 *            the number of factorizations of n * into factors > 1 (unordered,
 *            1 not allowed)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* helpers */
#include "../utils/alloc_cache.h"   /* seq_t *alloc_cache(idx_t n); */
#include "../utils/free_cache.h"    /* void free_cache(seq_t *p); */

static seq_t *f_cache = NULL;          /* cached f values, index 0..n */
static idx_t f_cache_cap = 0;          /* current allocated capacity */

/* ensure cache can hold index n */
static void ensure_factor_cache(idx_t n)
{
    if (n < f_cache_cap) return;
    size_t new_cap = (size_t)n + 1;    /* index n must be accessible */
    seq_t *new_cache = alloc_cache(new_cap);
    if (f_cache != NULL) {
        memcpy(new_cache, f_cache, f_cache_cap * sizeof(seq_t));
        free_cache(f_cache);
    }
    /* zero-initialize new part */
    memset(new_cache + f_cache_cap, 0, (new_cap - f_cache_cap) * sizeof(seq_t));
    f_cache = new_cache;
    f_cache_cap = new_cap;
}

/* recursive count of unordered factorizations of n with minimum factor ≤min */
static seq_t count_factorizations(idx_t n, idx_t min_factor)
{
    seq_t res = (seq_t)1;           /* the factorization [n] itself */
    for (idx_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            res += count_factorizations(n / d, d);
        }
    }
    return res;
}

/* retrieve cached or compute f(n) */
static seq_t get_factor_count(idx_t n)
{
    ensure_factor_cache(n);
    if (f_cache[n] != 0) return f_cache[n];
    seq_t val;
    if (n == 1) {
        val = (seq_t)1;
    } else {
        val = count_factorizations(n, 2);
    }
    f_cache[n] = val;
    return val;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sum = (seq_t)0;

    /* divisor d = n itself */
    sum += get_factor_count(n);          /* f(n) * f(1) with f(1)=1 */

    /* other divisors d < n */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;                 /* complementary divisor */
            seq_t fd = get_factor_count(d);
            seq_t fe = get_factor_count(e);
            sum += fd * fe;                  /* term for divisor d */
            if (e != d) {                    /* term for divisor e */
                sum += fe * fd;
            }
        }
    }
    return sum;
}
