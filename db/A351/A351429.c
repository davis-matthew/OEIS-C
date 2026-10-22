/*
 * A351429  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. 1/(1 + f^k(x)), where f(x) =
 *          exp(x) - 1.
 * https://oeis.org/A351429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A351429:  square array T(n,k) read by antidiagonals
 *
 *  T(n,0) = (-1)^n * n!                     (for all n ≥ 0)
 *  T(n,k) = ∑_{j=0..n} Stirling2(n,j) * T(j,k-1)   (for k > 0)
 *
 *  a(n) is the sequence obtained by reading the array by
 *  antidiagonals:  a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0), a(3)=T(0,2), …
 *
 *  The implementation precomputes all values needed for
 *  n ≤ 10000.  The maximum diagonal index (max_s) is 141,
 *  but we use a safety margin of 200 to simplify the code.
 *
 *  Only integer arithmetic is used; seq_t is assumed to
 *  support normal +, -, *, / operators.  No external libraries
 *  beyond the C/POSIX standard headers are needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of the required function */
seq_t a(idx_t n);

/* ---------- internal utilities ---------- */

/* integer square root: floor(sqrt(v)) for v >= 0 */
static inline idx_t floor_isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = 1ULL << 32;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > v / mid) /* mid*mid > v, avoid overflow */
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* ---------- lazy initialization of the tables ---------- */

#define MAX_S 200          /* 200 > required diagonal index for n ≤ 10000 */

static bool initialized = false;
static seq_t *T = NULL;     /* array T[n*(MAX_S+1)+k] */
static seq_t *S = NULL;     /* Stirling2[n*(MAX_S+1)+j] */
static seq_t *fact = NULL;  /* factorials fact[n] */

static void init_tables(void)
{
    if (initialized) return;
    const idx_t dim = MAX_S + 1;
    T = (seq_t*)malloc(dim * dim * sizeof(seq_t));
    S = (seq_t*)malloc(dim * dim * sizeof(seq_t));
    fact = (seq_t*)malloc(dim * sizeof(seq_t));
    if (!T || !S || !fact) {
        /* if allocation fails, abort the program; in a real
           library we would handle this more gracefully. */
        exit(1);
    }

    /* factorials */
    fact[0] = 1;
    for (idx_t i = 1; i < dim; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling numbers of the second kind, S[n][j] */
    for (idx_t n = 0; n < dim; ++n) {
        for (idx_t j = 0; j <= n; ++j) {
            if (n == 0 && j == 0)
                S[n * dim + j] = 1;
            else if (j == 0)
                S[n * dim + j] = 0;
            else
                S[n * dim + j] = S[(n - 1) * dim + (j - 1)] + /* S(n-1,j-1) */
                                 (seq_t)j * S[(n - 1) * dim + j]; /* j*S(n-1,j) */
        }
    }

    /* T[n][0] = (-1)^n * n! */
    for (idx_t n = 0; n < dim; ++n)
        T[n * dim + 0] = ((n & 1) ? -fact[n] : fact[n]);

    /* Recurrence for k > 0 */
    for (idx_t k = 1; k < dim; ++k) {
        for (idx_t n = 0; n < dim; ++n) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= n; ++j)
                sum += S[n * dim + j] * T[j * dim + (k - 1)];
            T[n * dim + k] = sum;
        }
    }

    initialized = true;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    init_tables();

    /* find diagonal index s such that n ∈ [s*(s+1)/2 , (s+1)*(s+2)/2 - 1] */
    idx_t tmp = 8 * n + 1;
    idx_t sqrt_val = floor_isqrt_u64(tmp);
    idx_t s = (sqrt_val - 1) >> 1;          /* (sqrt_val-1)/2 */

    /* starting index of this diagonal */
    idx_t diag_start = (s * (s + 1)) >> 1;  /* s*(s+1)/2 */

    /* position within the diagonal */
    idx_t i = n - diag_start;          /* row index */
    idx_t j = s - i;                  /* column index */

    const idx_t dim = MAX_S + 1;
    return T[i * dim + j];
}
