/*
 * A375057  a(n) = least k such that (n*pi)^(2k)/(2 k)! < 1.
 * https://oeis.org/A375057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (n*pi)^(2k)/(2 k)! < 1
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static double *LOGFACT = NULL;
static const size_t LOGFACT_MAX = 2000000; /* up to 1e6 k (since 2k <= 2e6) */

static void init_logfact(void)
{
    static bool init = false;
    if (!init) {
        LOGFACT = (double *)malloc((LOGFACT_MAX + 1) * sizeof(double));
        LOGFACT[0] = 0.0;
        for (size_t i = 1; i <= LOGFACT_MAX; ++i)
            LOGFACT[i] = LOGFACT[i - 1] + log((double)i);
        init = true;
    }
}

static double f(idx_t k, double lnval)
{
    size_t idx = 2 * (size_t)k;
    return LOGFACT[idx] - 2.0 * (double)k * lnval;
}

seq_t a(idx_t n)
{
    init_logfact();
    if (n == 0)
        return (seq_t)1;

    const double PI = 3.14159265358979323846264338327950288;
    double lnval = log((double)n * PI);

    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound where f(high) > 0 */
    while (2 * (size_t)high <= LOGFACT_MAX && f(high, lnval) <= 0.0)
        high <<= 1;

    /* Binary search for minimal k with f(k)>0 */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (f(mid, lnval) > 0.0)
            high = mid;
        else
            low = mid;
    }
    return (seq_t)high;
}
