/* binomial: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_H
#define OEISC_UTILS_BINOMIAL_H
#include <stdint.h>

/* C(n, k), exact whenever the result fits in seq_t: each step computes
   C(n-k+i, i) = C(n-k+i-1, i-1) * (n-k+i) / i as (r/g) * ((n-k+i)/(i/g)) with
   g = gcd(r mod i, i), so no intermediate value exceeds the next coefficient. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; i++) {
        idx_t g = i, x = (idx_t)(r % (seq_t)i);
        while (x != 0) {
            idx_t t = g % x;
            g = x;
            x = t;
        }
        r = (r / (seq_t)g) * (seq_t)((n - k + i) / (i / g));
    }
    return r;
}

#endif
