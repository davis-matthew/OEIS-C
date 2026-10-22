/*
 * A359660  a(n) = Sum_{k=0..n} k^(2 * (n-k) + 1).
 * https://oeis.org/A359660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^(2*(n-k)+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*
 * The implementation keeps two static arrays:
 *   terms[k]  = k^(2*(cur - k)+1)  for the current value of cur
 *   cache[n]  = a(n) computed so far
 *
 * It grows incrementally: when a larger n is requested, the arrays are
 * updated to that value using only integer arithmetic.
 *
 * The size is limited to 10000, which covers the sequence usage in the
 * test harness.  The arrays are static so that the function works
 * correctly regardless of the order of calls.
 */
#if !defined(MAX_A_N)
#define MAX_A_N 10000
#endif

static idx_t cur_max = 0;                     /* largest n already computed */
static seq_t cache[MAX_A_N+1];                /* cache[n] = a(n) */
static seq_t terms[MAX_A_N+1];                /* current powers k^(2*(cur-k)+1) */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Handle initial state */
    if (cur_max == 0)
    {
        /* initialize base values for n = 0 */
        cache[0] = (seq_t)0;
        terms[0] = (seq_t)0;   /* not used */
        cur_max = 0;
    }

    /* Ensure we have enough room for the requested n */
    if (n > MAX_A_N)
        return ((seq_t)0); /* larger n not supported – should not happen */

    /* Expand precomputed values if needed */
    if (n > cur_max)
    {
        /* compute for each cur from cur_max+1 up to n */
        for (idx_t cur = cur_max + 1; cur <= n; ++cur)
        {
            seq_t sum = (seq_t)0;

            /* update existing terms k = 1 .. cur-1 */
            for (idx_t k = 1; k < cur; ++k)
            {
                seq_t k_sq = (seq_t)k * (seq_t)k;     /* k^2 */
                terms[k] *= k_sq;                    /* multiply by k^2 */
                sum += terms[k];
            }

            /* term for k = cur (exponent 1) */
            terms[cur] = (seq_t)cur;
            sum += terms[cur];

            /* store a(cur) */
            cache[cur] = sum;
        }

        cur_max = n;
    }

    return cache[n];
}
