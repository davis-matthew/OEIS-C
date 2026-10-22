/*
 * A355248  Number of ways to write n as the sum of (exactly) 3 positive
 *          integers with the same number of divisors.
 * https://oeis.org/A355248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} * [ d(j)
 *            = d(i) = d(n-i-j) ] * where d(k) is the number of positive
 *            divisors of k. The brackets * denote the Iverson bracket,
 *            evaluating to 1 if its condition is * true and 0 otherwise. This
 *            counts unordered triples of positive * integers (i...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: no positive integers can sum to 0,1,2 */
    if (n < 3) return (seq_t)0;

    /* Allocate array for divisor counts 1..n */
    idx_t *d = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!d) return (seq_t)0;                /* allocation failure → 0 */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) d[i] = 0;

    /* classic sieve to count divisors */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++d[j];
        }
    }

    seq_t count = 0;
    idx_t j, i, k;
    for (j = 1; j <= n / 3; ++j) {
        idx_t dj = d[j];
        idx_t max_i = (n - j) / 2;   /* ensures k >= i */
        for (i = j; i <= max_i; ++i) {
            if (d[i] != dj) continue;
            k = n - i - j;
            if (k < i) continue;   /* should never happen, safety */
            if (d[k] == dj) ++count;
        }
    }

    free(d);
    return count;
}
