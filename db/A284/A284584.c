/*
 * A284584  a(1) = 0; for n > 1, if n is not squarefree, then a(n) =
 *          A057627(n), otherwise a(n) = A013928(n).
 * https://oeis.org/A284584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, * a(n)=A013928(n)-1 if n is squarefree, *
 *            a(n)=A057627(n) otherwise, * where * A013928(n)=∑_{k=1}^{⌊√n⌋}
 *            μ(k)⌊n/k²⌋ (count of squarefree ≤ n), * A057627(n)=n-A013928(n)
 *            (count of non‑squarefree ≤ n). * This is equivalent to the
 *            description in the OEIS entry
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root, floor, without <math.h>. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            best = mid;
            low = mid + 1;
        }
    }
    return best;
}

/* Möbius function μ(k).  k≥1. */
static int8_t mu_of(idx_t k)
{
    if (k == 1) return 1;
    int8_t sign = 1;
    idx_t temp = k;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            temp /= p;
            if ((temp % p) == 0) return 0;
            sign = -sign;
            while (temp % p == 0) temp /= p;           /* remove all occurrences of p */
        }
    }
    if (temp > 1) sign = -sign;    /* remaining prime factor */
    return sign;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* base case, matches A013928(1)-1 */

    /* Count squarefree numbers ≤ n: Q = A013928(n) */
    idx_t sqrt_n = isqrt_u64(n);
    long long q_ll = 0;
    for (idx_t k = 1; k <= sqrt_n; ++k) {
        int8_t mu = mu_of(k);
        if (mu != 0) {
            idx_t denom = k * k;
            q_ll += mu * (long long)(n / denom);
        }
    }

    /* Determine squarefreeness of n via μ(n) */
    bool squarefree = (mu_of(n) != 0);

    if (squarefree) {
        /* a(n) = Q – 1  (A013928(n) – 1) */
        return (seq_t)(q_ll - 1);
    } else {
        /* a(n) = n – Q  (A057627(n)) */
        return (seq_t)(n - (seq_t)q_ll);
    }
}
