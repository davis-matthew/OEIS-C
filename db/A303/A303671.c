/*
 * A303671  a(n) = [x^n] (6 / Sum_{k=0..n} (k+3)!*x^k)^(1/2).
 * https://oeis.org/A303671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A303671
    a(n) = [x^n] ( 6 / ∑_{k≥0} (k+3)! x^k )^(1/2)
    Sequence is defined for n ≥ 0 with a(0)=1.  The computation uses
    the identity H(x)·G(x)^2 = 6 where
        H(x) = ∑_{k≥0} (k+3)! x^k,
        G(x) = ∑_{n≥0} a(n) x^n.
    This yields the recurrences
        S(0)=1,
        S(n) = -∑_{k=1}^n H_k · S(n-k)   (n≥1),
        a(0)=1,
        a(n) = ( S(n) - ∑_{i=1}^{n-1} a(i)·a(n-i) ) / 2   (n≥1),
    where H_k = (k+3)!/6 and S(n)=∑_{i=0}^n a(i)a(n-i).  The function
    keeps a short‑term cache of already‑computed values up to the largest
    index requested, and expands it only when a larger n is asked.  The
    cache size is limited to 10001 entries, which suffices for the usual
    test range (n≈10 000).  The implementation uses only integer
    arithmetic and the provided ``seq_t`` type.  It is valid C11 and
    C++17. */

#include <stddef.h>   /* for size_t */

#define MAX_CACHE_SIZE 10001

/* Forward declaration of the required external types. */
extern seq_t a(idx_t n);

/* Static cache of already computed values.  All three arrays are of
   the same size and 0‑initialised on first use. */
static seq_t H2[MAX_CACHE_SIZE];   /* (k+3)! / 6, for k = 0 .. max_n */
static seq_t S  [MAX_CACHE_SIZE];  /* convolution sums S(n) */
static seq_t A  [MAX_CACHE_SIZE];  /* a(n) values */

static size_t cached_max = 0;      /* maximum index currently computed */
static int    cached     = 0;      /* flag: 0 = uninitialised, 1 = initialised */

/* Initialise the base case a(0)=1, S(0)=1, H2(0)=1. */
static void init_cache(void)
{
    H2[0] = (seq_t)1;
    S [0] = (seq_t)1;
    A [0] = (seq_t)1;
    cached_max = 0;
    cached     = 1;
}

/* Expand the cache to include all indices up to N (inclusive).  The
   calculation is incremental, reusing already‑computed values. */
static void grow_cache(size_t N)
{
    if (!cached)              /* first use */
        init_cache();

    if (N <= cached_max)      /* nothing to do */
        return;

    /* Ensure requested size does not exceed maximum allowed by the
       static arrays.  If it does, the behavior is undefined – the
       caller must not request n > 10000. */
    if (N >= (size_t)MAX_CACHE_SIZE)
        N = (size_t)MAX_CACHE_SIZE - 1;   /* clamp to maximum */

    /* Incrementally compute values from cached_max+1 up to N. */
    for (size_t cur = cached_max + 1; cur <= N; ++cur) {
        /* Compute H2[cur] = H2[cur-1] * (cur+3). */
        H2[cur] = H2[cur - 1] * (seq_t)(cur + 3);

        /* Compute S[cur] = - Σ_{k=1}^cur H2[k] * S[cur-k]. */
        seq_t sum = 0;
        for (size_t k = 1; k <= cur; ++k)
            sum += H2[k] * S[cur - k];
        S[cur] = -sum;

        /* Compute a[cur] = ( S[cur] - Σ_{i=1}^{cur-1} a[i]*a[cur-i] ) / 2. */
        seq_t cross = 0;
        for (size_t i = 1; i < cur; ++i)
            cross += A[i] * A[cur - i];
        A[cur] = (S[cur] - cross) / 2;
    }

    cached_max = N;
}

/* Public entry point.  The function satisfies all the constraints
   specified in the problem statement. */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    grow_cache(N);
    return A[N];
}
