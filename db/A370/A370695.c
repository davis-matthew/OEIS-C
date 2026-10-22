/*
 * A370695  G.f. A(x) satisfies A(x) = (1 + x*A(x)^(3/4) / (1-x))^4.
 * https://oeis.org/A370695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrences derived from A370695: a(0) = 1
 *            a(1) = 4 a(2) = 22 a(3) = 128 For n ≥ 4 a(n) = ( (55n²+74n+15)
 *            *a(n-1) - 6(37n²-46n-4) *a(n-2) + (295n-319)(n-3) *a(n-3) -
 *            124(n-3)(n-4) *a(n-4) ) / ( 2(n+2)(2n+3) ) All operations use
 *            exact arithmetic; intermediate results may exceed...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Note: seq_t and idx_t are defined by the building system.
   idx_t is an unsigned integer type big enough to index the array.
   seq_t is an unsigned integer type or an arbitrary‑precision integer
   type; all arithmetic operators are overloaded appropriately. */

static seq_t *cache = NULL;       /* memoisation array */
static size_t cap = 0;            /* allocated capacity */
static idx_t last = 3;            /* highest index computed so far */

static const seq_t init[4] = { (seq_t)1, (seq_t)4, (seq_t)22, (seq_t)128 };

/* Ensure the cache is large enough and initialise the first four terms. */
static void ensure_cache(idx_t n)
{
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failure – abort silently */
            return;
        cache = tmp;
        cap = new_cap;
        /* initialise the initial values once */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    }
}

seq_t a(idx_t n)
{
    if ((n <= 3))
        return init[n];

    ensure_cache(n);

    /* compute missing values up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t a1 = cache[k - 1];
        seq_t a2 = cache[k - 2];
        seq_t a3 = cache[k - 3];
        seq_t a4 = cache[k - 4];

        /* coefficients (all fit in 64 bit on typical builds);
           cast to seq_t for consistency. */
        seq_t k_seq = (seq_t)k;
        seq_t coeff1 = (seq_t)(55 * k_seq * k_seq + 74 * k_seq + 15);
        seq_t coeff2 = (seq_t)(6 * (37 * k_seq * k_seq - 46 * k_seq - 4));
        seq_t coeff3 = (seq_t)((295 * k_seq - 319) * (k_seq - 3));
        seq_t coeff4 = (seq_t)(124 * (k_seq - 3) * (k_seq - 4));

        /* 128‑bit intermediates to avoid overflow */
        signed __int128 t1 = (__int128)coeff1 * (__int128)a1;
        signed __int128 t2 = (__int128)coeff2 * (__int128)a2;
        signed __int128 t3 = (__int128)coeff3 * (__int128)a3;
        signed __int128 t4 = (__int128)coeff4 * (__int128)a4;

        signed __int128 numerator   = t1 - t2 + t3 - t4;
        signed __int128 denominator =
            (__int128)(2 * (k_seq + 2) * (2 * k_seq + 3));

        /* Exact division: the result is an integer. */
        seq_t value = (seq_t)(numerator / denominator);
        cache[k] = value;
    }
    last = n;
    return cache[n];
}
