/*
 * A336022  a(0) = ... = a(4) = 1; a(n) = Sum_{k=0..n-5} Stirling2(n-5,k) *
 *          a(k).
 * https://oeis.org/A336022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 19, u64 n <= 28, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336022
   a(0) = a(1) = a(2) = a(3) = a(4) = 1;
   for n >= 5:
       a(n) = Σ_{k=0}^{n-5} S(n-5, k) * a(k)
   where S(m, k) is the Stirling number of the second kind. */

/* The function below builds the sequence on demand, using only
   the integer types seq_t and idx_t provided by the build. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *A        = NULL;  /* cached a-values */
static size_t Acap    = 0;
static idx_t  Asize   = 0;      /* next index to compute */

static seq_t *S_prev   = NULL;  /* Stirling row m */
static seq_t *S_cur    = NULL;  /* temporary row */
static size_t Scap    = 0;
static idx_t  Sm      = 0;     /* current m for S_prev */

/* Grow Stirling buffer to at least 'need' elements. */
static void grow_stir_cap(idx_t need)
{
    if (Scap <= need) {
        size_t new_cap = Scap ? Scap << 1 : 1;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        S_prev = (seq_t*)realloc(S_prev, new_cap * sizeof(seq_t));
        S_cur  = (seq_t*)realloc(S_cur,  new_cap * sizeof(seq_t));
        Scap = new_cap;
    }
}

/* Ensure that a[0..n] are computed.  Called by a(). */
static void ensure_a(idx_t n)
{
    if (Asize > n) return;          /* nothing to do */

    /* Grow A array if necessary */
    if (Acap <= (size_t)n) {
        size_t new_cap = Acap ? Acap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        Acap = new_cap;
    }

    /* Initialize Stirling if not yet done */
    if (S_prev == NULL) {
        Scap = 1;
        S_prev = (seq_t*)malloc(Scap * sizeof(seq_t));
        S_cur  = (seq_t*)malloc(Scap * sizeof(seq_t));
        S_prev[0] = (seq_t)1;      /* S(0,0) = 1 */
        Sm = 0;                    /* current m */
    }

    /* Compute a[a.size .. n] */
    for (idx_t i = Asize; i <= n; ++i) {
        if (i <= (idx_t)4) {          /* initial values */
            A[(size_t)i] = (seq_t)1;
        } else {
            idx_t m = i - 5;          /* row of Stirling numbers needed */

            /* Build Stirling row m if we haven't yet */
            if (m > Sm) {
                for (idx_t cur = Sm + 1; cur <= m; ++cur) {
                    grow_stir_cap(cur);
                    S_cur[0] = (seq_t)0;
                    for (idx_t k = cur; k >= 1; --k) {
                        seq_t value_k = (k <= Sm) ? S_prev[k] : (seq_t)0;
                        S_cur[k] = S_prev[k-1] + (seq_t)k * value_k;
                    }
                    /* swap rows */
                    seq_t *tmp = S_prev;
                    S_prev = S_cur;
                    S_cur  = tmp;
                    Sm = (idx_t)cur;
                }
            }

            /* Compute a[i] = Σ_{k=0}^{m} S_prev[k] * a[k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                sum += S_prev[(size_t)k] * A[(size_t)k];
            }
            A[(size_t)i] = sum;
        }
    }
    Asize = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return A[(size_t)n];
}
