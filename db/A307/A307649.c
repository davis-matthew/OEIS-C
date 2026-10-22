/*
 * A307649  G.f. A(x) satisfies: (1 + x) = A(x)*A(x^2)^2*A(x^3)^3*A(x^4)^4*
 *          ... *A(x^k)^k* ...
 * https://oeis.org/A307649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 22, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A307649: a(n) defined by the generating function 
 *   ∏_{k≥1} (1 + x^k)^{μ(k)·k},
 * where μ is the Möbius function.
 * 
 * We use the logarithmic expansion
 *   log A(x) = ∑_{n≥1} S_n x^n / n,
 * with  
 *   S_n = ∑_{d|n} μ(d)·d²·(-1)^{n/d-1}.
 * 
 * The exponential can be computed by the recurrence
 *   n·a(n) = ∑_{k=1}^n S_k · a(n-k),   a(0)=1.
 * 
 * The implementation precomputes μ, S and a up to the maximum n
 * requested in any call, and stores the values persistently to
 * support non‑increasing sequences of calls.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_arr = NULL;          /* a values */
static seq_t *s_arr = NULL;          /* S values */
static int   *mu_arr = NULL;         /* Möbius values */

static size_t capacity = 0;           /* current allocated size */
static size_t max_computed = 0;       /* highest n already computed */

/* Free previously allocated memory */
static void free_memory(void)
{
#ifdef __cplusplus
    delete[] a_arr;
    delete[] s_arr;
    delete[] mu_arr;
#else
    free(a_arr);
    free(s_arr);
    free(mu_arr);
#endif
    a_arr = NULL;
    s_arr = NULL;
    mu_arr = NULL;
    capacity = 0;
}

/* Allocate or grow the arrays to hold at least n+1 elements */
static void grow_capacity(idx_t n)
{
    if (n < capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    free_memory();
#ifdef __cplusplus
    a_arr = new seq_t[new_cap]();  /* value‑initialise (zero) */
    s_arr = new seq_t[new_cap]();
    mu_arr = new int[new_cap]();
#else
    a_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    s_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    mu_arr = (int*)calloc(new_cap, sizeof(int));
#endif
    capacity = new_cap;
}

/* Compute Möbius function μ(k) for 1 ≤ k ≤ n using linear sieve */
static void compute_mobius(idx_t n)
{
    bool *is_comp = (bool*)calloc(n + 1, sizeof(bool));
    int  *primes  = (int *)malloc((n + 1) * sizeof(int));
    size_t pc = 0;
    mu_arr[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            mu_arr[i] = -1;
        }
        for (size_t j = 0; j < pc && i * (idx_t)primes[j] <= n; ++j) {
            idx_t v = i * (idx_t)primes[j];
            is_comp[v] = true;
            if (i % primes[j] == 0) {
                mu_arr[v] = 0;
                break;
            } else {
                mu_arr[v] = -mu_arr[i];
            }
        }
    }
    free(is_comp);
    free(primes);
}

/* Compute S_k for 1 ≤ k ≤ n */
static void compute_S(idx_t n)
{
    for (idx_t k = 1; k <= n; ++k)
        s_arr[k] = 0;  /* initialise */

    for (idx_t d = 1; d <= n; ++d) {
        if (mu_arr[d] == 0) continue;
        seq_t mu_d = (seq_t)mu_arr[d];
        seq_t d_sq = (seq_t)d * (seq_t)d;
        for (idx_t k = d; k <= n; k += d) {
            idx_t mult = k / d;               /* k = d * mult */
            int exp = (int)(mult - 1);        /* exponent of (-1) */
            seq_t sgn = (exp & 1) ? (seq_t)-1 : (seq_t)1;
            s_arr[k] += mu_d * d_sq * sgn;
        }
    }
}

/* Compute a_n for all 0 ≤ n ≤ N using the recurrence */
static void compute_a(idx_t N)
{
    a_arr[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            sum += s_arr[k] * a_arr[n - k];
        }
        a_arr[n] = sum / (seq_t)n;  /* exact division */
    }
    max_computed = N;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n > max_computed) {
        grow_capacity(n);
        compute_mobius(n);
        compute_S(n);
        compute_a(n);
    }
    return a_arr[n];
}
