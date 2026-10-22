/*
 * A300153  Square array T(n, k) read by antidiagonals upwards, n > 0 and k >
 *          0: T(n, k) is the number of parts inscribed in a rose or rhodonea
 *          curve with polar coordinates r = cos(t * (k/n)).
 * https://oeis.org/A300153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(1, k) = A022998(k); T(n, k) = T(n/gcd(n,k), k/gcd(n,k)). If
 *            gcd(n, k) = 1, then - k odd, n odd : T(n, k) = k * A029578(n+1)
 *            - k odd, n even: T(n, k) = 2*k * A029578(n+1) - k even, n odd:
 *            T(n, k) = 2*k*n Here A029578(m) = (m-1) if m odd, or m/2 if m
 *            even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   A029578, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>          /* for bool */
#include "../utils/gcd.h"     /* gcd(seq_t,seq_t) */
#include "../utils/A029578.h"

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

/* Computes T(n,k) as defined above. */
static seq_t compute_T(idx_t n, idx_t k)
{
    idx_t g = (idx_t)gcd((seq_t)n, (seq_t)k);
    if (g > 1)                   /* reduce the pair */
        return compute_T(n / g, k / g);

    /* now gcd(n,k) == 1 */
    if (n == 1) {                 /* A022998(k) */
        return (k & 1) ? (seq_t)k : (seq_t)(2 * k);
    }

    if ((k & 1) == 0) {           /* k even, n must be odd */
        return (seq_t)(2 * k * n);
    }
    /* k odd */
    if ((n & 1) == 0) {           /* n even */
        return (seq_t)(2 * k * A029578(n + 1));
    }
    /* n odd */
    return (seq_t)k * A029578(n + 1);
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for index 0, but safe guard */

    /* Find the diagonal S such that (S-1)*S/2 >= n */
    idx_t M = 1 + 8 * n;
    idx_t sqrtM = isqrt_u64(M);
    idx_t S = (1 + sqrtM) >> 1;
    __int128 cum = (__int128)(S - 1) * S / 2;
    while (cum < n) {
        ++S;
        cum = (__int128)(S - 1) * S / 2;
    }

    /* Count of terms before diagonal S */
    idx_t prior = ((S - 2) * (S - 1)) / 2;   /* (S-2)*(S-1)/2 */
    idx_t offset = n - prior;                /* 1-based offset on diagonal */
    idx_t k_val = offset;                    /* k = offset on this diagonal */
    idx_t n_val = S - offset;                /* n = S - offset */

    return compute_T(n_val, k_val);
}
