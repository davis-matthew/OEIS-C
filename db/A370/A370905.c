/*
 * A370905  Partial sums of the alternating sum of divisors function
 *          (A206369).
 * https://oeis.org/A370905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * floor(sqrt(n/k))
 * Range:     u8 n <= 27, u16 n <= 445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* integer square‑root on 64‑bit unsigned integers, no overflow. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0)
        {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid > n / mid)          /* mid*mid > n, avoid overflow */
            high = mid - 1;
        else
        {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* a(n) = Sum_{k=1..n} phi(k) * floor(sqrt(n/k)) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    /* Allocate array for φ(0..n) */
    idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!phi)
        return (seq_t)0;           /* allocation failure */

    /* Initialize φ(i) = i */
    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;

    /* Sieve to compute Euler's totient function */
    for (idx_t p = 2; p <= n; ++p)
        if (phi[p] == p)           /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / p;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = n / k;
        idx_t t = isqrt_u64(m);   /* floor(sqrt(m)) */
        sum += (seq_t)phi[k] * (seq_t)t;
    }

    free(phi);
    return sum;
}
