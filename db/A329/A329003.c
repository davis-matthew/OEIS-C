/*
 * A329003  Numbers k such that the coefficient of x^k in the expansion of
 *          Product_{j>=2} (1 - x^Fibonacci(j)) is zero.
 * https://oeis.org/A329003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that the coefficient of x^k in ∏_{j≥2} (1 -
 *            x^{F_j}) is zero
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS sequence A329003
   (1‑based indexing) by generating the coefficients of the
   product over distinct Fibonacci numbers and collecting the
   positions where the coefficient is zero.  The implementation
   uses a basic subset‑sum DP and doubles the search range until
   enough zeros are found. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *zeros = NULL;        /* array of found zero positions */
    static size_t zeros_cap = 0;       /* capacity of zeros array */
    static size_t zeros_len = 0;       /* number of zeros stored */
    static int initialized = 0;        /* whether state has been initialised */

    if (!initialized) {
        zeros_cap = 256;
        zeros = (seq_t *)malloc(zeros_cap * sizeof(seq_t));
        zeros_len = 0;
        initialized = 1;
    }

    /* Start with a search range that is likely large enough.
       If not enough zeros are found, double the range. */
    size_t Kmax = (size_t)n * 4 + 128;
    if (Kmax < 256) Kmax = 256;

    while (1) {
        /* Allocate DP array for sums up to Kmax */
        long long *dp = (long long *)malloc((Kmax + 1) * sizeof(long long));
        if (!dp) return 0; /* allocation failure – unlikely in test harness */
        dp[0] = 1;
        for (size_t i = 1; i <= Kmax; ++i) dp[i] = 0;

        /* Generate Fibonacci numbers 1,2,3,5,... up to Kmax and update DP */
        size_t f1 = 1, f2 = 2, f = f1;
        while (f <= Kmax) {
            /* Update DP for current Fibonacci number f */
            for (size_t s = Kmax; s >= f; --s) {
                dp[s] -= dp[s - f];
            }
            /* Advance to next Fibonacci number */
            size_t f_next = f1 + f2;
            f1 = f2;
            f2 = f_next;
            f = f1;
        }

        /* Collect all positions where coefficient is zero */
        size_t count = 0;
        /* Ensure zeros array is large enough */
        if (zeros_cap < Kmax + 1) {
            size_t new_cap = Kmax + 1;
            seq_t *tmp = (seq_t *)realloc(zeros, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(dp);
                return 0; /* allocation failure */
            }
            zeros = tmp;
            zeros_cap = new_cap;
        }
        for (size_t k = 1; k <= Kmax; ++k) {
            if (dp[k] == 0) {
                if (count == zeros_cap) {
                    size_t new_cap = zeros_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(zeros, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(dp);
                        return 0; /* allocation failure */
                    }
                    zeros = tmp;
                    zeros_cap = new_cap;
                }
                zeros[count++] = (seq_t)k;
            }
        }
        zeros_len = count;
        free(dp);

        /* Have we found enough terms? */
        if (zeros_len >= (size_t)n) break;

        /* Not enough yet – double the search range */
        Kmax *= 2;
    }

    return zeros[n - 1];
}
