/*
 * A370525  Number of permutations of [n] having exactly one adjacent 3-cycle.
 * https://oeis.org/A370525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)-1} (-1)^k * (n-2k-2)! / k!
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation caches factorials up to the largest n requested.
    It works for any integer type seq_t (fixed‑size or arbitrary precision)
    and for any idx_t type provided.  No external factorial routine is used. */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of required types.  These are supplied by the build
   environment (seq.h or similar).  Do not define them here. */
typedef uint64_t idx_t;  /* placeholder – actual type will be supplied */

/* ----------  Local cache for factorials  ---------- */
static seq_t *fact = NULL;          /* fact[i] = i! for 0 <= i <= fact_n */
static idx_t    fact_n = 0;         /* highest index whose factorial is known */

/* Compute factorials up to `n`, extending the cache as needed.  */
static void ensure_factorial_cache(idx_t n)
{
    if (n <= fact_n)
        return;                      /* cache already up to date */

    /* If the cache exists, enlarge it; otherwise create it. */
    size_t new_size = (size_t)(n + 1);
    if (fact == NULL) {
        fact = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!fact) { /* allocation failure: abort program, but in a
                          real environment this should be handled differently */
            abort();
        }
        fact[0] = (seq_t)1;           /* 0! == 1 */
    } else {
        fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        if (!fact) abort();
    }

    /* Compute missing factorials incrementally. */
    for (idx_t i = fact_n + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_n = n;
}

/* ----------  Sequence a(n)  ---------- */
seq_t a(idx_t n)
{
    /* The summation range is empty for n < 3. */
    if (n < 3)
        return (seq_t)0;

    idx_t kmax = n / 3;          /* floor(n/3) */
    if (kmax == 0)
        return (seq_t)0;
    kmax--;                      /* floor(n/3)-1 */

    /* We will need factorials up to n. */
    ensure_factorial_cache(n);

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t term_idx = n - 2 * k - 2;   /* (n-2k-2)! */
        seq_t term = fact[term_idx] / fact[k];  /* exact division */

        if (k & 1)  /* odd k: subtract */
            sum -= term;
        else        /* even k: add */
            sum += term;
    }
    return sum;
}
