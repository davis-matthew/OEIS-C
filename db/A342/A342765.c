/*
 * A342765  Array T(n, k), n, k > 0, read by antidiagonals; T(n, k) =
 *          max(A006530(n), A006530(k)) * T(n/A006530(n), k/A006530(k)) with
 *          T(1, 1) = 1.
 * https://oeis.org/A342765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1144, u16 n <= 9267, u32 n <= 9267, u64 n <= 9267,
 *            u128 n <= 9267, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(i,j) where i+j = d+1 and d is the minimal integer
 * satisfying d*(d+1)/2 >= n.
 * T(n,k) is defined recursively:
 *   T(1,1)=1,   T(n,1)=n,   T(1,k)=k,
 *   T(n,k)=max(lpf(n),lpf(k))*T(n/lpf(n),k/lpf(k))  for n>1 && k>1.
 *
 * The implementation computes the antidiagonal coordinates (i,j)
 * for the index n and evaluates T(n,k) using the largest prime factor
 * of each argument.
 */

#include <stdbool.h>

/* largest prime factor of n (n>=1), returns 1 for n==1 */
static inline idx_t largest_prime_factor(idx_t n)
{
    if (n <= 1) return 1;
    idx_t last = 1;
    while ((n & 1ULL) == 0) {
        last = 2;
        n >>= 1;
    }
    for (idx_t d = 3; d * d <= n; d += 2) {
        while (n % d == 0) {
            last = d;
            n /= d;
        }
    }
    if (n > 1) last = n;
    return last;
}

/* compute T(n,k) using the recursive definition */
static inline seq_t T_of(idx_t n, idx_t k)
{
    seq_t result = 1;
    while (n > 1 && k > 1) {
        idx_t p1 = largest_prime_factor(n);
        idx_t p2 = largest_prime_factor(k);
        idx_t m = (p1 > p2) ? p1 : p2;
        result *= (seq_t)m;
        n /= p1;
        k /= p2;
    }
    if (n == 1) result *= (seq_t)k;
    else if (k == 1) result *= (seq_t)n;
    return result;
}

/* find the minimal d such that d*(d+1)/2 >= n */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n == 0 */

    idx_t d = diagonal_of(n);          /* d*(d+1)/2 >= n */
    idx_t before = (d - 1) * d / 2;    /* elements preceding this diagonal */
    idx_t offset = n - before - 1;     /* zero‑based index within diagonal */
    idx_t i = offset + 1;              /* row index */
    idx_t j = (d + 1) - i;             /* column index (i + j = d + 1) */

    return T_of(i, j);
}
