/*
 * A354507  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d+1) * d )/(k *
 *          (n-k)!).
 * https://oeis.org/A354507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} ( Σ_{d|k} (-1)^(k/d+1) * d ) / (k *
 *            (n-k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal cache for A000593(k) (sum over divisors with alternating signs) */
static long long *A_long = NULL;
/* Factorial cache */
static seq_t *fact_arr = NULL;
/* Largest n that the caches cover */
static idx_t max_n = 0;

/* Ensure that A_long[1..n] and fact_arr[0..n] are computed */
static void ensure_arrays(idx_t n) {
    if (n <= max_n) return;

    idx_t new_n = n;
    size_t new_size = (size_t)new_n + 1;            /* +1 for 0..n */

    /* Resize A_long */
    A_long = (long long *)realloc(A_long, new_size * sizeof(long long));
    /* New entries are zeroed */
    for (idx_t i = max_n + 1; i <= new_n; ++i) A_long[i] = 0;

    /* Resize factorial array */
    fact_arr = (seq_t *)realloc(fact_arr, new_size * sizeof(seq_t));
    if (max_n == 0) fact_arr[0] = (seq_t)1;          /* base case */
    for (idx_t i = max_n + 1; i <= new_n; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }

    /* Update contributions for new indices */
    for (idx_t d = 1; d <= new_n; ++d) {
        idx_t start_k;
        if (d <= max_n) {
            start_k = ((max_n / d) + 1) * d;        /* first multiple of d > max_n */
        } else {
            start_k = d;                            /* d itself is new */
        }
        if (start_k > new_n) continue;
        for (idx_t k = start_k; k <= new_n; k += d) {
            idx_t m = k / d;
            int sign = (m & 1) ? 1 : -1;           /* (-1)^(m+1) */
            if (sign == 1)
                A_long[k] += (long long)d;
            else
                A_long[k] -= (long long)d;
        }
    }

    max_n = new_n;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_arrays(n);

    seq_t result = (seq_t)0;
    seq_t binom   = (seq_t)1;   /* C(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1) * (n-k+1)/k */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / (seq_t)k;

        seq_t term = ((seq_t)A_long[k]) * binom;
        term = term * fact_arr[k - 1];
        result += term;
    }
    return result;
}
