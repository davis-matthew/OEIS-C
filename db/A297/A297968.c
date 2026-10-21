/*
 * A297968  Number of solutions to x*y*(x+y)=n in coprime integers.
 * https://oeis.org/A297968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A297968: number of ordered coprime integer solutions (x,y)
   to the equation x*y*(x+y)=n.  The sequence counts each ordered pair
   separately, even when x=y (e.g. n=2 has the pair (1,1) counted twice).  */

/* Helper: integer square root for seq_t (non‑negative). */
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Count solutions for a fixed integer x (signed). */
static seq_t count_for_x(idx_t n, int64_t xv)
{
    /* C = n / x must be integer. */
    int64_t C = (int64_t)n / xv;
    /* Discriminant Δ = x² + 4C. */
    int64_t D = xv * xv + 4 * C;
    if (D < 0) return 0;
    seq_t Dseq = (seq_t)D;
    seq_t k = isqrt_seq(Dseq);
    if (k * k != Dseq) return 0;
    int64_t kll = (int64_t)k;

    seq_t total = 0;
    /* First root: y = (-x + k)/2 */
    int64_t num1 = -xv + kll;
    if (num1 % 2 == 0)
    {
        int64_t y1 = num1 / 2;
        seq_t ax = (seq_t)(xv >= 0 ? xv : -xv);
        seq_t ay = (seq_t)(y1 >= 0 ? y1 : -y1);
        if (gcd(ax, ay) == 1)
        {
            seq_t inc = 1;
            if (xv == y1) inc += 1;   /* double counting when x == y */
            total += inc;
        }
    }

    /* Second root: y = (-x - k)/2, avoid duplicate when k == 0 */
    if (kll != 0)
    {
        int64_t num2 = -xv - kll;
        if (num2 % 2 == 0)
        {
            int64_t y2 = num2 / 2;
            seq_t ax = (seq_t)(xv >= 0 ? xv : -xv);
            seq_t ay = (seq_t)(y2 >= 0 ? y2 : -y2);
            if (gcd(ax, ay) == 1)
            {
                seq_t inc = 1;
                if (xv == y2) inc += 1;   /* double counting when x == y */
                total += inc;
            }
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe guard */
    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            result += count_for_x(n, (int64_t)d);
            result += count_for_x(n, -(int64_t)d);
        }
    }
    return result;
}
