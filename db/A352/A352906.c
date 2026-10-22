/*
 * A352906  Expansion of e.g.f. sinh(x) / (1 - sin(x)).
 * https://oeis.org/A352906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n,2*k+1) *
 *            A000111(n-2*k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

static seq_t* euler = NULL;        /* Euler up/down numbers A000111 */
static size_t euler_cap = 0;       /* capacity of allocated array */
static size_t euler_len = 0;       /* number of elements currently stored */

/* Ensure euler[0..need] are computed. */
static void ensure_euler(idx_t need) {
    if (euler_len > 0 && need < euler_len) return;

    size_t new_cap = (euler_cap > 0) ? euler_cap : 1;
    while (new_cap <= need) new_cap <<= 1;

    seq_t *new_arr = NULL;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap]();
#else
    new_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif
    if (euler) {
        for (size_t i = 0; i < euler_len; ++i) new_arr[i] = euler[i];
#ifdef __cplusplus
        delete[] euler;
#else
        free(euler);
#endif
    }
    euler = new_arr;
    euler_cap = new_cap;

    /* Initialize base cases if they don't exist yet */
    if (euler_len == 0) {
        euler[0] = 1;
        euler_len = 1;
    }
    if (need >= 1 && euler_len == 1) {
        euler[1] = 1;
        euler_len = 2;
    }

    /* Compute missing Euler numbers using recurrence:
       E_n = (1/2) * sum_{k=0}^{n-1} binomial(n-1,k) * E_k * E_{n-1-k} */
    for (idx_t i = euler_len; i <= need; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i-1; ++k) {
            seq_t b = binomial(i-1, k);
            sum += b * euler[k] * euler[i-1-k];
        }
        euler[i] = sum / 2;
    }
    euler_len = need + 1;
}

/* Main function computing the sequence */
seq_t a(idx_t n) {
    /* The formula gives a(0) = 0 automatically */
    if (n == 0) return (seq_t)0;

    /* Make sure Euler numbers up to n are available */
    ensure_euler(n);

    seq_t res = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t odd = 2*k + 1;
        if (odd > n) break;
        idx_t euler_idx = n - 2*k;                       /* n - 2k */
        seq_t b = binomial(n, odd);
        res += b * euler[euler_idx];
    }
    return res;
}
