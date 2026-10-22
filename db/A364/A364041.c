/*
 * A364041  Expansion of 1/Product_{k>0} (1 - x^(2*k-1))^((2*k-1)^(2*k-1)).
 * https://oeis.org/A364041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364041
   a(0) = 1, a(n) = (1/n) * Σ_{k=1..n} A363991(k) * a(n-k)
   where A363991(k) = Σ_{d odd | k} d^(d+1).
   This implementation memoises computed terms and recomputes the
   required divisor sums on demand. */
 
#include <stdlib.h>
#include <stddef.h>
 
/* Helper: power by squaring, works for builtin and arbitrary‐precision seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
 
/* Helper: ensure capacity of a dynamic array */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr) {
    if (*cap >= needed) return;
    size_t newcap = (*cap == 0) ? 8 : *cap * 2;
    while (newcap < needed) newcap *= 2;
    seq_t *newp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    if (!newp) abort();   /* out of memory */
    *ptr = newp;
    *cap = newcap;
}
 
/* Memoised storage for a(n), A(n), and d^(d+1) values */
static seq_t *a_cache = NULL;      /* a[n] values */
static size_t a_cap = 0, a_len = 0;/* a_cache size and number of computed entries */
 
static seq_t *A_cache = NULL;      /* A[n] divisor sums */
static size_t A_cap = 0, A_len = 0;/* A_cache size and number of computed entries */
 
static seq_t *pow_table = NULL;    /* d^(d+1) for odd d */
static size_t pow_cap = 0, pow_last = 0;/* maximal odd d computed */
 
/* Ensure A_cache and pow_table contain values up to n */
static void ensure_A_up_to(idx_t n) {
    if (A_len > n) return;          /* already up to date */
 
    /* Allocate pow_table up to n (only odd indices used) */
    ensure_capacity((size_t)n + 1, &pow_cap, &pow_table);
    /* Allocate A_cache up to n */
    ensure_capacity((size_t)n + 1, &A_cap, &A_cache);
 
    /* Initialise new entries */
    for (size_t i = A_len; i <= (size_t)n; ++i) A_cache[i] = (seq_t)0;
 
    /* Old maximum index already processed */
    size_t old_max = A_len - 1;
 
    /* Process odd d up to n */
    for (idx_t d = 1; d <= n; d += 2) {
        /* Compute d^(d+1) if not already */
        if ((size_t)d > pow_last) {
            pow_table[d] = pow_seq((seq_t)d, d + 1);
            pow_last = (size_t)d;
        }
        seq_t val = pow_table[d];
 
        /* Determine first multiple > old_max */
        size_t m_start = d * ((old_max / d) + 1);
        for (size_t m = m_start; m <= (size_t)n; m += (size_t)d) {
            A_cache[m] += val;
        }
    }
 
    /* Update for next calls */
    A_len = (size_t)n + 1;
    /* Ensure a_cache has capacity at least as large */
    ensure_capacity(A_len, &a_cap, &a_cache);
    /* Initialise a_cache[0] */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }
}
 
/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* Ensure we have computed up to n */
    ensure_A_up_to(n);
 
    /* Compute a values up to n (if not already) */
    if (a_len <= n) {
        for (idx_t i = a_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += A_cache[k] * a_cache[i - k];
            }
            /* Division by i is exact by construction */
            a_cache[i] = sum / (seq_t)i;
        }
        a_len = n + 1;
    }
    return a_cache[n];
}
