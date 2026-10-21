/*
 * A280248  Partial products of A280246, where A280246(n) = Product_{d|n}
 *          A023896(d).
 * https://oeis.org/A280248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=1}^n \; \prod_{d|i} A023896(d) where A023896(d)
 *            = 1 if d==1, else d*phi(d)/2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t max_n = 0;
static idx_t *phi_arr = NULL;
static seq_t *s_arr   = NULL;
static seq_t *a_arr   = NULL;

/* compute phi for all 0..N */
static void compute_phi_upto(idx_t N)
{
    /* allocate phi array */
    if (!phi_arr) phi_arr = (idx_t*)calloc(N + 1, sizeof(idx_t));
    else            phi_arr = (idx_t*)realloc(phi_arr, (N + 1) * sizeof(idx_t));

    for (idx_t i = 0; i <= N; ++i) phi_arr[i] = i;

    for (idx_t i = 2; i <= N; ++i) {
        if (phi_arr[i] == i) {               /* i is prime */
            for (idx_t j = i; j <= N; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }
}

/* compute s[d] = A023896(d) for all 0..N */
static void compute_s_upto(idx_t N)
{
    if (!s_arr) s_arr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    else        s_arr = (seq_t*)realloc(s_arr, (N + 1) * sizeof(seq_t));

    s_arr[0] = (seq_t)0;
    s_arr[1] = (seq_t)1;                       /* special case */
    for (idx_t d = 2; d <= N; ++d) {
        seq_t phi = (seq_t)phi_arr[d];
        s_arr[d] = ((seq_t)d * phi) / (seq_t)2;
    }
}

/* compute a[n] progressively up to N */
static void compute_a_upto(idx_t N)
{
    if (!a_arr) a_arr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    else        a_arr = (seq_t*)realloc(a_arr, (N + 1) * sizeof(seq_t));

    for (idx_t i = max_n + 1; i <= N; ++i) {
        seq_t prod = (seq_t)1;          /* A280246(i) */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                prod *= s_arr[d];
                idx_t other = i / d;
                if (other != d)
                    prod *= s_arr[other];
            }
        }
        if (i == 1) a_arr[1] = prod;    /* a(1) = s(1) */
        else        a_arr[i] = a_arr[i - 1] * prod;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    if (n > max_n) {
        compute_phi_upto(n);
        compute_s_upto(n);
        compute_a_upto(n);
        max_n = n;
    }
    return a_arr[n];
}
