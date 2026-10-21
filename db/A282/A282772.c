/*
 * A282772  Starting from F(n), minimum number, greater than 1, of consecutive
 *          Fibonacci numbers whose average is an integer.
 * https://oeis.org/A282772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimum m > 1 such that (F(n+m+1) – F(n+1)) mod m = 0.
 *            The sequence repeats with period 420. The implementation
 *            pre‑computes one period by brute force, using only operations
 *            that fit into the available seq_t type
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t period[420];
static bool built = false;

/* Fibonacci numbers modulo m, for m = 2 … 24.  The table is indexed by
   (m-2) and contains values up to index 444 (419 + 24 + 1). */
#define MAX_M 24
#define MAX_N 444          /* 419 + 24 + 1 */
static seq_t fib_mod[MAX_M-1][MAX_N+1];

static void build_period(void)
{
    /* Compute fib modulo each m. */
    for (int m = 2; m <= MAX_M; ++m) {
        int idx = m - 2;
        seq_t mod = (seq_t)m;
        fib_mod[idx][0] = 0;
        fib_mod[idx][1] = 1 % mod;
        for (int i = 2; i <= MAX_N; ++i) {
            fib_mod[idx][i] = (fib_mod[idx][i-1] + fib_mod[idx][i-2]) % mod;
        }

        /* For every start index n, find the minimal m giving an integer
           average. Non‑overlapping with other m’s because we break on the
           first hit. */
        for (size_t n = 0; n < 420; ++n) {
            /* Only compute period[n] once, per m. */
            if (period[n] == 0) {
                /* Compute (F(n+m+1) – F(n+1)) mod m. */
                seq_t diff = (fib_mod[idx][n+m+1] + mod
                              - fib_mod[idx][n+1]) % mod;
                if (diff == 0) {
                    period[n] = (seq_t)m;
                }
            }
        }
    }

    /* For any n that was not set by the loop above (shouldnot happen), set
       a default value (largest m).  This protects the code against future
       changes in the sequence definition. */
    for (size_t i = 0; i < 420; ++i) {
        if (period[i] == 0) {
            period[i] = (seq_t)MAX_M;
        }
    }

    built = true;
}

seq_t a(idx_t n)
{
    if (!built) {
        build_period();
    }
    return period[(size_t)(n % 420)];
}
