/*
 * A394115  G.f. A(x) satisfies A(x)^3+(2x-2)*A(x)^2+(1-x)*A(x)+x^2-x=0.
 * https://oeis.org/A394115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A394115: cubic generating function
 * A(x)^3 + (2x-2)A(x)^2 + (1-x)A(x) + x^2 - x = 0.
 * Recurrence derived from coefficient comparison:
 * a(0)=0, a(1)=1,
 * a(n) = a(n-1) + 2*c2(n) - 2*c2(n-1) - c3(n) - delta
 * where
 *   c2(k) = sum_{i=1}^{k-1} a[i] * a[k-i]
 *   c3(k) = sum_{i=1}^{k-1} a[i] * c2(k-i)
 *   delta = -1 if k==1, +1 if k==2, 0 otherwise.
 * This computes all terms using O(n^2) arithmetic.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;       /* a[n] */
static seq_t *c2_cache = NULL;     /* c2[n] */
static idx_t cap = 0;               /* allocated capacity */
static idx_t max_n_computed = 0;    /* greatest n for which a[n] is known */

/* Ensure the caches are large enough and initialise the base cases */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;

    /* allocate new capacity (double the size or enough for n+1) */
    idx_t newcap = cap * 2;
    if (newcap < 16) newcap = 16;
    if (newcap <= n) newcap = n + 1;
    seq_t *newa = (seq_t*)malloc(newcap * sizeof(seq_t));
    seq_t *newc2 = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (a_cache) {
        for (idx_t i = 0; i < cap; ++i) {
            newa[i] = a_cache[i];
            newc2[i] = c2_cache[i];
        }
        free(a_cache);
        free(c2_cache);
    }
    /* zero initialise the new part */
    for (idx_t i = cap; i < newcap; ++i) {
        newa[i] = 0;
        newc2[i] = 0;
    }
    a_cache = newa;
    c2_cache = newc2;
    cap = newcap;

    /* initialise base values if they are within the new capacity */
    if (max_n_computed == 0) {           /* not yet initialised */
        a_cache[0] = 0;
        c2_cache[0] = 0;
        if (cap > 1) {
            a_cache[1] = 1;
            c2_cache[1] = 0;
        }
        max_n_computed = 1;
    }
}

/* Compute all terms up to "n" using the recurrence */
static void compute_up_to(idx_t n)
{
    if (n <= max_n_computed)
        return;

    ensure_capacity(n);

    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        /* compute c2[k] */
        seq_t c2k = 0;
        for (idx_t i = 1; i < k; ++i)
            c2k += a_cache[i] * a_cache[k - i];
        c2_cache[k] = c2k;

        /* compute c3[k] */
        seq_t c3k = 0;
        for (idx_t i = 1; i < k; ++i)
            c3k += a_cache[i] * c2_cache[k - i];

        /* a[k] = a[k-1] + 2*c2[k] - 2*c2[k-1] - c3[k] - delta */
        seq_t val = a_cache[k - 1];

        seq_t term4 = c2_cache[k];
        term4 += term4;          /* 2 * c2[k] */
        seq_t term3 = c2_cache[k - 1];
        term3 += term3;          /* 2 * c2[k-1] */

        val += term4;
        val -= c3k;
        val -= term3;

        if (k == 1)           /* delta = -1 -> add 1 */
            val += 1;
        else if (k == 2)      /* delta = +1 -> subtract 1 */
            val -= 1;

        a_cache[k] = val;
    }

    max_n_computed = n;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_up_to(n);
    return a_cache[n];
}
