/*
 * A378468  Coefficients in expansion of (1/E_4)^3.
 * https://oeis.org/A378468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 7, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = coefficient of q^n in the formal power series
   expansion of (1/E_4(q))^3,
   where E_4(q) = 1 + 240*∑_{m≥1} σ_3(m)*q^m.  The recurrence below
   computes the coefficients for all n ≥ 0.  It uses
   E_4(q)^-3 = (1 + 240∑σ_3(m)q^m)^-3.  Since 1 + X has inverse
   ∑_{k≥0} (-X)^k, we first invert the series for E_4 up to the
   required index, then cube it by two convolutions.  Complexity
   is O(n^2) and is fast enough for the typical n ≤ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* The types `seq_t` and `idx_t` are supplied by the build system. */
typedef uint64_t idx_t;          /* index type: unsigned 64‑bit integer */

/* Allocate enough space for all indices up to (but not including) MAX_N.
   The problem guarantees n ≤ 10 000. */
#define MAX_N 10001             /* 0 … 10000 inclusive */

/* Working arrays – static so that constructors (if any) are run once. */
static seq_t G[MAX_N];          /* 1/E_4 coefficients   */
static seq_t H[MAX_N];          /* (1/E_4)^2 coefficients */
static seq_t A[MAX_N];          /* desired sequence (1/E_4)^3 */
static uint64_t sigma3[MAX_N];  /* σ_3(n) values         */
static int computed_up_to = -1; /* highest n computed so far */

/* ------------------------------------------------------------------ */
/* Helper to initialise the σ_3 array up to N.                       */
/* ------------------------------------------------------------------ */
static void compute_sigma3(idx_t N)
{
    /* ensure the array has enough space – MAX_N guarantees that */
    if (N > (idx_t)(MAX_N - 1))
        N = MAX_N - 1;

    /* zero the needed part first */
    for (idx_t i = 1; i <= N; ++i)
        sigma3[i] = 0;

    /* divisor summatory loop */
    for (idx_t d = 1; d <= N; ++d) {
        uint64_t d3 = (uint64_t)d * (uint64_t)d * (uint64_t)d;
        for (idx_t m = d; m <= N; m += d)
            sigma3[m] += d3;
    }
}

/* ------------------------------------------------------------------ */
/* Compute all values up to index N.  Results are stored in the global
   arrays.  The routine is idempotent for already computed indices.  */
/* ------------------------------------------------------------------ */
static void compute_up_to(idx_t N)
{
    if ((int)N <= computed_up_to)
        return;                 /* nothing new to do */

    /* keep within bounds */
    if (N >= (idx_t)(MAX_N - 1))
        N = MAX_N - 1;

    /* σ_3 values */
    compute_sigma3(N);

    /* -------------------------------------------------------------------
       Compute G = 1/E_4  by formal power series inversion:
                           G[0] = 1
                           G[n] = -∑_{k=1..n} f_k * G[n-k]
       where f_k = 240 * σ_3(k).                                           
       ------------------------------------------------------------------- */
    G[0] = (seq_t)1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t f_k = (seq_t)((uint64_t)240 * sigma3[k]);
            sum += f_k * G[n - k];
        }
        G[n] = -sum;
    }

    /* -------------------------------------------------------------------
       Compute H = G * G  (convolution).                                  
       ------------------------------------------------------------------- */
    for (idx_t i = 0; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= i; ++j)
            sum += G[j] * G[i - j];
        H[i] = sum;
    }

    /* -------------------------------------------------------------------
       Compute A = (G * G) * G  = H * G.                                 
       ------------------------------------------------------------------- */
    for (idx_t n = 0; n <= N; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= n; ++k)
            sum += H[k] * G[n - k];
        A[n] = sum;
    }

    computed_up_to = (int)N;
}

/* ------------------------------------------------------------------ */
/* Public function required by the runner.                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* the problem guarantees that n is a non‑negative integer */
    compute_up_to(n);
    return A[(idx_t) n];
}
