/*
 * A357727  Expansion of e.g.f. cos( 2 * (exp(x) - 1) ).
 * https://oeis.org/A357727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-4)^k * Stirling2(n, 2*k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum n expected (≈10000). Adjust if higher n are needed. */
#define MAX_N 10000

/* Static arrays big enough for the maximum required n. */
static seq_t S[MAX_N + 2];          /* Stirling numbers, up to index n+1 */
static seq_t powP[(MAX_N / 2) + 3]; /* (-4)^k, up to k = floor(n/2) + 1 */
static seq_t cache[MAX_N + 1];      /* a(n) values */

static int  initialized = 0; /* indicates if initialization has been done */
static idx_t last_computed = 0;  /* highest n for which cache[n] is valid */
static idx_t powP_used = 1;      /* number of powP values already computed */

/* Computes a(n) for all n ≤ n.  */
static void ensure_n(idx_t n)
{
    if (n <= last_computed)
        return;          /* requested value already computed */

    /* Increase cache size if necessary (static array is large enough). */

    /* Compute missing values up to n.  */
    while (last_computed < n) {
        idx_t next = last_computed + 1;     /* next n to compute */

        /* Make sure powP contains all powers up to floor(next/2). */
        while (powP_used <= next / 2) {
            powP[powP_used] = powP[powP_used - 1] * (-4);
            ++powP_used;
        }

        /* a(next) = Sum_{k=0}^{floor(next/2)} (-4)^k * S[2k]   */
        seq_t sum = 0;
        for (idx_t k = 0; k <= next / 2; ++k)
            sum += powP[k] * S[2 * k];

        cache[next] = sum;      /* store result */

        /* Update Stirling numbers to the next row (n = next + 1). */
        /* S currently holds Stirling numbers of row 'next'. */
        /* Compute new values in-place from high to low indices. */
        for (idx_t k = next + 1; k > 0; --k)
            S[k] = (seq_t)k * S[k] + S[k - 1];
        S[0] = 0;                /* S(…,0) = 0 for n > 0 */

        ++last_computed;        /* one more term computed */
    }
}

/* Public implementation of the sequence. */
seq_t a(idx_t n)
{
    /* Initialization on first call. */
    if (!initialized) {
        /* All static arrays are zero-initialized. */
        S[0] = 1;                        /* S(0,0) = 1      */
        powP[0] = 1;                     /* (-4)^0 = 1      */
        cache[0] = 1;                    /* a(0) = 1        */
        /* Prepare Stirling numbers for n = 1. */
        S[1] = S[0];                     /* Stirling(1,1) = 1 */
        S[0] = 0;
        initialized = 1;
        last_computed = 0;                /* only a(0) computed so far */
    }

    /* Compute missing terms up to n. */
    if (n > MAX_N)
        return 0; /* safety guard – out of bounds */

    ensure_n(n);

    return cache[n];
}
