/*
 * A256533  Product of n and the sum of all divisors of all positive integers
 *          <= n.
 * https://oeis.org/A256533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{k=1}^n σ(k) where σ(k) is the sum of divisors of
 *            k
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 1734, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *g_prefix = NULL;            /* prefix sums of σ(i) */
static size_t g_capacity = 0;              /* computed up to this index */

/* Compute σ(i) for all 1 ≤ i ≤ up_to and store prefix sums in g_prefix */
static void recompute_prefix(size_t up_to)
{
#ifdef __cplusplus
    /* Use C++ new/new[] for arbitrary‑precision types, zero-initialized */
    seq_t *divsum = new seq_t[up_to + 1]();
    seq_t *tmp_pref = new seq_t[up_to + 1]();

    for (idx_t i = 1; i <= up_to; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= up_to; j += i)
            divsum[j] += add;
    }

    seq_t running = 0;
    for (idx_t i = 1; i <= up_to; ++i) {
        running += divsum[i];
        tmp_pref[i] = running;
    }
    tmp_pref[0] = 0;

    delete[] divsum;
    if (g_prefix) delete[] g_prefix;
    g_prefix = tmp_pref;
#else
    /* Use C malloc/calloc for building in C mode */
    seq_t *divsum = calloc(up_to + 1, sizeof(seq_t));
    seq_t *tmp_pref = calloc(up_to + 1, sizeof(seq_t));

    for (idx_t i = 1; i <= up_to; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= up_to; j += i)
            divsum[j] += add;
    }

    seq_t running = 0;
    for (idx_t i = 1; i <= up_to; ++i) {
        running += divsum[i];
        tmp_pref[i] = running;
    }
    tmp_pref[0] = 0;

    free(divsum);
    if (g_prefix) free(g_prefix);
    g_prefix = tmp_pref;
#endif
}

/* External interface required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n<1, though not needed */

    if ((size_t)n > g_capacity) {
        recompute_prefix((size_t)n);
        g_capacity = (size_t)n;
    }

    /* a(n) = n * Σ_{k=1}^n σ(k) = n * g_prefix[n] */
    return (seq_t)n * g_prefix[n];
}
