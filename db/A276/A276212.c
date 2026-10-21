/*
 * A276212  Position of n^r in the joint ranking of {h^r} and {k^s}, where r =
 *          sqrt(2), s = sqrt(5), h > 1, k > 1.
 * https://oeis.org/A276212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) + floor((n+1)^(sqrt(2/5)))
 * Range:     u8 n <= 226, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The exponent sqrt(2/5) is irrational; floor is computed by a binary
 * search that uses long double arithmetic for the comparison.
 * The result is exact for any n >= 1 and fits in seq_t for the usual
 * (up to 64‑bit) builds. */
#include <math.h>

static const long double EXP_Q = sqrtl(2.0L / 5.0L);
static const long double EPS   = 1e-12L;   /* safety margin */

seq_t a(idx_t n)
{
    /* The OEIS offset starts at 1; for n = 0 we return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    /* x = n + 1 is the exponentiated base in the formula. */
    idx_t x = n + 1;

    /* Compute a floating‑point approximation of (n+1)^q for the search
     * initial bounds.  The value is small (< 400 for n ≤ 10 000). */
    long double val = powl((long double)x, EXP_Q);

    /* Binary search for the greatest integer m such that m^q <= val. */
    unsigned long long low  = 0;
    unsigned long long high = (unsigned long long)(val) + 1;  /* upper bound */

    while (low < high) {
        unsigned long long mid = (low + high) >> 1;       /* mid = floor((low+high)/2) */

        long double mid_pow = powl((long double)mid, EXP_Q);

        if (mid_pow <= val + EPS)
            low = mid + 1;      /* mid is feasible; look higher */
        else
            high = mid;         /* mid too large; look lower */
    }

    unsigned long long floor_val = low - 1;

    /* a(n) = (n-1) + floor((n+1)^q) */
    seq_t result = (seq_t)(n - 1) + (seq_t)(floor_val);
    return result;
}
