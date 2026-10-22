/*
 * A340675  Exponential of Mangoldt function conjugated by Tek's flip: a(n) =
 *          A225546(A014963(A225546(n))).
 * https://oeis.org/A340675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=1; a(n)=2 if n squarefree>1; a(n)=1 if n not
 *            square and not squarefree; a(n)=a(sqrt(n))^2 if n is a perfect
 *            square
 * Range:     u8 n <= 255, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t left = 1, right = n, sq = 0;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        if (mid <= n / mid) { sq = mid; left = mid + 1; }
        else { right = mid - 1; }
    }
    return sq;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t t = n;
    int all_one = 1;
    int all_even = 1;
    seq_t sqrt_val = (seq_t)1;

    while (t > 1) {
        idx_t p = 0;
        int count = 0;

        if (t % 2 == 0) {
            p = 2;
            while (t % 2 == 0) { t /= 2; ++count; }
        } else {
            idx_t limit = isqrt(t);
            for (idx_t i = 3; i <= limit; i += 2) {
                if (t % i == 0) {
                    p = i;
                    while (t % i == 0) { t /= i; ++count; }
                    break;
                }
            }
            if (p == 0) {
                p = t;
                count = 1;
                t = 1;
            }
        }

        if (count > 1) all_one = 0;
        if ((count & 1) != 0) all_even = 0;
        if ((count & 1) == 0) {
            int half = count >> 1;
            for (int i = 0; i < half; ++i) {
                sqrt_val = sqrt_val * (seq_t)p;
            }
        }
    }

    if (all_even) {
        seq_t val = a((idx_t) sqrt_val);
        return val * val;
    } else {
        return all_one ? (seq_t)2 : (seq_t)1;
    }
}
