/*
 * A337684  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s, t, nor (s + t) divides n.
 * https://oeis.org/A337684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ (s,t) | 1 ≤ s < t < n, n % s ≠ 0, n % t ≠ 0, n % (s+t)
 *            ≠ 0 }|
 * Range:     u8 n <= 28, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* no valid pairs when n < 3 */

    /* Allocate an array to mark the divisors of n (1 .. n). */
    unsigned char *div = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!div) return (seq_t)0;     /* allocation failure – unlikely in tests */
    memset(div, 0, (n + 1) * sizeof(unsigned char));

    /* Mark divisors of n. */
    idx_t r = 0;
    for (idx_t i = 1; i * i <= n; ++i) r = i;          /* integer sqrt */
    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            div[i] = 1;
            idx_t j = n / i;
            if (j != i) div[j] = 1;
        }
    }

    seq_t count = 0;
    for (idx_t s = 1; s <= n - 2; ++s) {
        if (div[s]) continue;                 /* s divides n -> skip */
        for (idx_t t = s + 1; t <= n - 1; ++t) {
            if (div[t]) continue;             /* t divides n -> skip */
            idx_t sum = s + t;
            if (sum <= n && div[sum]) continue; /* (s+t) divides n -> skip */
            ++count;
        }
    }

    free(div);
    return count;
}
