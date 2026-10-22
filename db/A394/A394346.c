/*
 * A394346  a(n) = tau(n) * pod(n) where tau(k) = the number of the divisors
 *          of k (A000005) and pod(k) = the product of the divisors of k
 *          (A007955).
 * https://oeis.org/A394346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) * pod(n) (number of divisors times product of
 *            divisors)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 59, u64 n <= 179, u128 n <= 719,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t sq = (seq_t)mid * (seq_t)mid;
        if (sq <= (seq_t)n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute tau(n) */
    seq_t tau = 1;
    idx_t tmp = n;

    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            e++;
        }
        tau *= (e + 1);
    }

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            tau *= (e + 1);
        }
    }

    if (tmp > 1) {          /* remaining prime */
        tau *= 2;
    }

    /* compute pod(n) = n^(tau/2) * (sqrt(n) if tau odd) */
    idx_t half = (idx_t)(tau / 2);
    seq_t pod = seq_pow((seq_t)n, half);

    if (tau % 2 == 1) {
        seq_t sqrt_n = (seq_t)isqrt_u64(n);
        pod *= sqrt_n;
    }

    return tau * pod;
}
