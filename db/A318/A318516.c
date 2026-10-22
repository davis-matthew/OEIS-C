/*
 * A318516  a(n) = A032742(n) OR n-A032742(n), where OR is bitwise-or
 *          (A003986) and A032742 = the largest proper divisor of n.
 * https://oeis.org/A318516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A032742(n) OR (n - A032742(n))
 * Range:     u8 n <= 255, u16 n <= 9897, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032742(n) = largest proper divisor of n
   For n = 1, we use 1 as the divisor (to satisfy a(1)=1).      */
#include <stdlib.h>
#include <stdint.h>

/*  Least–prime‑factor table (spf[0] unused, spf[1] = 1).  */
static uint64_t *spf = NULL;
static idx_t spf_max = 0;

/*  Ensure that spf array is filled for all numbers ≤ n.  */
static void ensure_spf_upto(idx_t n)
{
    if (spf != NULL && n <= spf_max)
        return;                       /* already enough */

    size_t sz = (size_t)n + 1;
    uint64_t *new_spf = (uint64_t *)calloc(sz, sizeof(uint64_t));
    if (new_spf == NULL)
        return;                       /* allocation failed – caller will fail */

    new_spf[0] = 0;
    new_spf[1] = 1;                    /* convention for n==1 */

    for (idx_t i = 2; i <= n; ++i) {
        if (new_spf[i] == 0) {         /* i is prime */
            new_spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
                }
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset 1 – undefined for 0 */

    ensure_spf_upto(n);

    seq_t N      = (seq_t)n;
    seq_t spf_n  = (seq_t)spf[n];       /* small prime factor, 1 for n==1 */
    seq_t large  = N / spf_n;           /* largest proper divisor */
    seq_t diff   = N - large;           /* n - largestProperDivisor */
    return large | diff;                /* bitwise OR */
}
