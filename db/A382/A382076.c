/*
 * A382076  Number of integer partitions of n whose run-sums are not all
 *          equal.
 * https://oeis.org/A382076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - Σ_{d|n} C(τ(d), n/d) where p(n) is the partition
 *            function and τ(d) is the number of positive divisors of d
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies:  <stddef.h>, <stdint.h>, <stdlib.h> */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Forward declaration for external linkage */
seq_t a(idx_t n);

/* Helper: binomial coefficient for small integers */
static seq_t binom_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Static storage for partitions and divisor counts */
static seq_t *p_arr = NULL;      /* partition numbers p[0..max_n] */
static size_t *tau_arr = NULL;   /* divisor counts τ[0..max_n] */
static idx_t max_n = 0;          /* current maximum n computed */

/* Ensure that partition and divisor arrays are computed up to N */
static void precompute(idx_t N)
{
    if (N <= max_n) return;
    #define REALLOC(arr, type, newsize) \
        do { \
            type *tmp = (type *)realloc((arr), (newsize) * sizeof(type)); \
            if (!tmp) exit(EXIT_FAILURE); \
            (arr) = tmp; \
        } while(0)

    /* Allocate / resize arrays */
    REALLOC(p_arr, seq_t, N + 1);
    REALLOC(tau_arr, size_t, N + 1);

    /* Initialize to zero (safety) */
    for (idx_t i = 0; i <= N; ++i) {
        p_arr[i] = 0;
        tau_arr[i] = 0;
    }

    /* Compute divisor counts τ[1..N] */
    for (idx_t d = 1; d <= N; ++d) {
        for (idx_t m = d; m <= N; m += d) {
            ++tau_arr[m];
        }
    }

    /* Compute partition numbers p[0..N] using Euler’s recurrence */
    p_arr[0] = 1;   /* p(0) = 1 */
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            if (k & 1) sum += p_arr[n - pent1];
            else       sum -= p_arr[n - pent1];
            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            if (k & 1) sum += p_arr[n - pent2];
            else       sum -= p_arr[n - pent2];
        }
        p_arr[n] = sum;
    }

    max_n = N;
    #undef REALLOC
}

/* Main function: sequence a(n) */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* a(0)=a(1)=a(2)=0 per definition */

    precompute(n);

    seq_t pn = p_arr[n];

    seq_t q = 0;   /* Count of partitions with equal run-sums */

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;   /* complementary divisor */

        /* Process divisor d */
        {
            idx_t g = n / d;                /* number of runs */
            size_t m = tau_arr[d];          /* number of divisors of d */
            if (g <= m) q += binom_small((int)m, (int)g);
        }

        /* Process complementary divisor e if distinct */
        if (e != d) {
            idx_t g = n / e;                /* number of runs for divisor e */
            size_t m = tau_arr[e];
            if (g <= m) q += binom_small((int)m, (int)g);
        }
    }

    return pn - q;
}
