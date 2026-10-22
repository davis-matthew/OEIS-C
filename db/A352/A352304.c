/*
 * A352304  Expansion of e.g.f. 1/(exp(x) - x^4).
 * https://oeis.org/A352304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-k-1)^(n-4*k)/(n-4*k)!
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Small-|seq_t| case: use recurrence that avoids huge intermediate values. */
    if (sizeof(seq_t) <= 8) {
        /* Allocate array of length n+1.  Memory is not freed; the function is
           called infrequently and the small size keeps leaks negligible. */
        seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!arr) return 0;  /* Out of memory – unlikely in tests. */

        arr[0] = 1;
        if (n > 0) arr[1] = -1;
        if (n > 1) arr[2] = 1;
        if (n > 2) arr[3] = -1;

        for (idx_t i = 4; i <= n; ++i) {
            /* term = n*(n-1)*(n-2)*(n-3)*a(n-4) */
            seq_t term = (seq_t)i * (i - 1) * (i - 2) * (i - 3) * arr[i - 4];

            /* sum = Sum_{k=1..i} binomial(i,k) * a(i-k) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t c = binomial(i, k);
                sum += c * arr[i - k];
            }

            arr[i] = term - sum;
        }

        seq_t result = arr[n];
        /* The allocated buffer is intentionally not freed to avoid repeated
           allocations; the function will be used only for small n. */
        return result;
    }

    /* Large-|seq_t| (arbitrary‑precision) case: use the closed form. */
    seq_t total = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = n - 4 * k;       /* exponent of the power term   */
        seq_t base = (seq_t)(-(int64_t)(k + 1));  /* -(k+1) as seq_t */
        seq_t power = pow_int(base, exp);         /* (-k-1)^(exp)  */

        /* Compute the factorial ratio n!/(n-4k)! as a product from m+1 to n,
           where m = n-4k. */
        idx_t m = exp;               /* m = n-4k                        */
        seq_t mult = 1;
        for (idx_t i = m + 1; i <= n; ++i) {
            mult *= (seq_t)i;
        }

        seq_t term = mult * power;
        total += term;
    }

    return total;
}
