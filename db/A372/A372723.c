/*
 * A372723  Triangle read by rows: Column k has e.g.f. t^k / ((1 - t)^(k + 1)
 *          * exp(t)).
 * https://oeis.org/A372723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r*(r+1)/2 <= n < (r+1)*(r+2)/2 and T(r,k) =
 *            Σ_{j=k..r} (-1)^(r-j) * j! * binomial(r,j) * binomial(j,k)
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *fact_cache = NULL;      /* factorial cache */
static idx_t fact_cache_size = 0;     /* number of stored factorials */

/* Ensure that factorials up to n are computed and stored in fact_cache. */
static void ensure_factorial_cache_up_to(idx_t n)
{
    if (n < fact_cache_size)
        return;

    /* Allocate at least (n+1) entries. */
    idx_t new_size = n + 1;
    seq_t *p = NULL;

    if (fact_cache_size == 0) {
        /* First allocation: 0! = 1 */
        p = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!p) exit(1);
        p[0] = (seq_t)1;
        fact_cache_size = 1;
    } else {
        p = (seq_t*)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!p) exit(1);
    }

    /* Compute missing factorials. */
    for (idx_t i = fact_cache_size; i < new_size; ++i) {
        seq_t val = p[i - 1];
        val = val * (seq_t)i;   /* multiplication OK for seq_t */
        p[i] = val;
    }

    fact_cache = p;
    fact_cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    /* Loop is very short because r grows like sqrt(2n). */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;      /* column index within the row */

    ensure_factorial_cache_up_to(r);    /* factorials up to r */

    seq_t pos_sum = (seq_t)0;           /* sum of terms with positive sign */
    seq_t neg_sum = (seq_t)0;           /* sum of terms with negative sign */

    for (idx_t j = k; j <= r; ++j) {
        seq_t term = fact_cache[j];                 /* j!  */
        term = term * binomial(r, j);               /* * binomial(r,j) */
        term = term * binomial(j, k);               /* * binomial(j,k) */

        if (((r - j) & 1) == 0) {                  /* even exponent -> positive */
            pos_sum += term;
        } else {
            neg_sum += term;                       /* odd exponent -> negative */
        }
    }

    /* The result is non‑negative; pos_sum ≥ neg_sum. */
    seq_t result = pos_sum - neg_sum;
    return result;
}
