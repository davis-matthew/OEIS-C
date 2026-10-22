/*
 * A344333  Primitive side of squares of type 1 (A344331) that are tiled with
 *          squares of two different sizes so that the number of large or
 *          small squares is the same.
 * https://oeis.org/A344333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A344333 – Primitive side of squares of type 1
 *  Generates all values s = a*b*(a²+b²) with 1 ≤ a < b,
 *  gcd(a,b)=1, sorted increasingly.  The precomputation is done once
 *  and memoized.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_cmp.h"

static seq_t *values = NULL;          /* array of precomputed values   */
static size_t values_len = 0;         /* number of values */

static void precompute(void)
{
    if (values != NULL)               /* already computed           */
        return;

    /* Upper bound for a, b sufficient for n up to ≈10 000.
     * Roughly 0.304·LIMIT² coprime pairs (a,b). With LIMIT=250 we get >19 000.
     */
    const idx_t LIMIT = 250;
    size_t max_pairs = (size_t)LIMIT * (LIMIT - 1) / 2;

    values = (seq_t *)malloc(max_pairs * sizeof(seq_t));
    values_len = 0;

    for (idx_t a = 1; a < LIMIT; ++a) {
        seq_t aa = (seq_t)a;
        for (idx_t b = a + 1; b < LIMIT; ++b) {
            if (gcd(aa, (seq_t)b) != 1)
                continue;

            seq_t bb = (seq_t)b;
            seq_t s = aa * bb * (aa * aa + bb * bb);
            values[values_len++] = s;
        }
    }

    /* Sort the list of values. */
    qsort(values, values_len, sizeof(seq_t), seq_cmp);

    /* Remove duplicates (should not occur with coprime a,b, but be safe). */
    size_t uniq = 0;
    for (size_t i = 0; i < values_len; ++i) {
        if (uniq == 0 || seq_cmp(&values[uniq - 1], &values[i]) != 0)
            values[uniq++] = values[i];
    }
    values_len = uniq;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 0 */
        return 0;

    precompute();

    if ((size_t)n > values_len)     /* safety guard – should not happen   */
        return 0;

    return values[(size_t)(n - 1)];
}
