/*
 * A344524  a(n) = Sum_{1 <= i, j, k, l, m <= n} gcd(i,j,k,l,m).
 * https://oeis.org/A344524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * floor(n/k)^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7070,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by summing the terms directly using a sieve for phi. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *phi_buf = NULL;          /* buffer holding phi values up to current size */
static size_t phi_buf_size = 0;        /* current size of phi_buf */

/* Ensure that phi_buf contains φ(1) … φ(n) inclusive */
static void compute_phi_until(idx_t n)
{
    size_t N = (size_t)n;
    if (phi_buf_size <= N) {
        size_t new_size = N + 1;
        seq_t *tmp = (seq_t *)realloc(phi_buf, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) {
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – abort the program */
                abort();
            }
        }
        phi_buf = tmp;
        phi_buf_size = new_size;

        /* Initialise phi[i] = i */
        for (size_t i = 0; i < new_size; ++i) {
            phi_buf[i] = (seq_t)i;
        }

        /* Sieve to compute Euler’s totient function */
        for (size_t i = 2; i <= N; ++i) {
            if (phi_buf[i] == (seq_t)i) {          /* i is prime */
                for (size_t j = i; j <= N; j += i) {
                    phi_buf[j] = phi_buf[j] - phi_buf[j] / (seq_t)i;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    compute_phi_until(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t div = n / k;                 /* floor(n/k) */
        seq_t d = (seq_t)div;
        seq_t p = 1;
        for (int e = 0; e < 5; ++e) {
            p *= d;
        }
        sum += phi_buf[(size_t)k] * p;
    }

    return sum;
}
