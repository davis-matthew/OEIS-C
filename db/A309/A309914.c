/*
 * A309914  Distance from n to closest triangular number that is different
 *          from n.
 * https://oeis.org/A309914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = distance from n to the nearest triangular number that is
 *            different from n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 number, returning floor(sqrt(x)). */
static inline unsigned int isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0) {       /* avoid division by zero */
            res = 0;
            low = 1;
            continue;
        }
        if (mid > x / mid) {  /* mid*mid > x */
            high = mid - 1;
        } else {              /* mid*mid <= x */
            res = mid;
            low = mid + 1;
        }
    }
    return (unsigned int)res;
}

seq_t a(idx_t n)
{
    /* Find k such that T_k <= n < T_{k+1} where T_k = k(k+1)/2 */
    unsigned __int128 x = (unsigned __int128)8 * n + 1;
    idx_t sqrtx = isqrt128(x);
    idx_t k = (sqrtx - 1) >> 1;          /* (sqrtx - 1)/2 */

    seq_t Tk  = (seq_t)k * ((seq_t)k + 1) / 2;      /* T_k */
    idx_t kp1 = k + 1;
    seq_t Tk1 = (seq_t)kp1 * ((seq_t)kp1 + 1) / 2; /* T_{k+1} */

    if (Tk != (seq_t)n) {
        /* n is not triangular: distance to nearest of T_k and T_{k+1} */
        seq_t dist_down = (seq_t)n - Tk;          /* n >= Tk */
        seq_t dist_up   = Tk1 - (seq_t)n;
        return dist_down < dist_up ? dist_down : dist_up;
    } else {
        /* n is triangular: skip itself and use nearest distinct triangular */
        if (k == 0) {
            /* n == 0: nearest triangular is 1 */
            return Tk1 - (seq_t)n; /* = 1 */
        } else {
            idx_t km1 = k - 1;
            seq_t Tkm1 = (seq_t)km1 * ((seq_t)km1 + 1) / 2; /* T_{k-1} */
            seq_t dist_down = (seq_t)n - Tkm1;  /* n > Tkm1 */
            seq_t dist_up   = Tk1 - (seq_t)n;
            return dist_down < dist_up ? dist_down : dist_up;
        }
    }
}
