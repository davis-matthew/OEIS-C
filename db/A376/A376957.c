/*
 * A376957  a(n) = least k such that (n Pi/2)^(2k+1)/(2k+1)! < 1.
 * https://oeis.org/A376957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (n * π/2)^(2k+1)/(2k+1)! < 1. For n = 0
 *            the answer is defined to be 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

#define MAX_M 50001   /* maximum (2k+1) needed for n up to 10000 */

static long double ln_fact[MAX_M + 1];
static int ln_fact_initialized = 0;

static void init_ln_fact(void)
{
    if (ln_fact_initialized) return;
    ln_fact[0] = 0.0L;
    for (int i = 1; i <= MAX_M; ++i)
        ln_fact[i] = ln_fact[i - 1] + logl((long double)i);
    ln_fact_initialized = 1;
}

static const long double PI = 3.141592653589793238462643383279502884L;
static const long double PI_OVER_2 = PI / 2.0L;

seq_t a(idx_t n)
{
    /* special case n == 0 */
    if (n == 0) return (seq_t)1;

    init_ln_fact();

    long double ln_x = logl((long double)n * PI_OVER_2);

    for (uint64_t k = 0; ; ++k)
    {
        uint64_t m = 2 * k + 1;
        /* Safety check */
        if (m > MAX_M) /* should not happen for n <= 10000 */
            return (seq_t)k; /* fallback */

        long double ln_term = (long double)m * ln_x - ln_fact[(int)m];
        if (ln_term < 0.0L)
            return (seq_t)k;
    }
}
