/*
 * A304825  Sum of binomial(Y(2,p), 2) over the partitions p of n, where
 *          Y(2,p) is the number of part sizes with multiplicity 2 or greater
 *          in p.
 * https://oeis.org/A304825
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t even, 0 <= t <= n-6} (t/4 + 1) * p(n-6-t) where
 *            p(k) is the partition number and the sum starts at n = 6. This
 *            follows from the generating function G(q) = q^6
 *            /((1-q^2)(1-q^4)) * Prod_{j>=1} 1/(1-q^j)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *p_arr = NULL;      /* partition numbers p[0..N]     */
static seq_t *a_arr = NULL;      /* results a[0..N]               */
static size_t capacity = 0;      /* current allocated size         */
static idx_t  p_computed = 0;    /* highest index with valid p    */
static idx_t  a_computed = 0;    /* highest index with valid a    */

static void ensure_capacity(idx_t N)
{
    if (capacity > (size_t)N) return;
    size_t new_cap = capacity ? capacity << 1 : 16;
    while (new_cap <= (size_t)N) new_cap <<= 1;
    p_arr = (seq_t *)realloc(p_arr, new_cap * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    for (size_t i = capacity; i < new_cap; ++i) {
        p_arr[i] = 0;
        a_arr[i] = 0;
    }
    capacity = new_cap;
}

/* Compute partition numbers p[0..N] using the generalized pentagonal
   number theorem.  */
static void compute_partitions(idx_t N)
{
    if (p_computed >= N) return;
    ensure_capacity(N);
    p_arr[0] = 1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t term = p_arr[n - pent1];
            if (k & 1) sum += term; else sum -= term;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) continue;
            term = p_arr[n - pent2];
            if (k & 1) sum += term; else sum -= term;
        }
        p_arr[n] = sum;
    }
    p_computed = N;
}

/* Compute a[n] for n in (a_computed, N] using the derived convolution.
   Values for n < 6 are defined as 0. */
static void compute_a_sequence(idx_t N)
{
    if (a_computed >= N) return;
    ensure_capacity(N);
    if (p_computed < N) compute_partitions(N);
    for (idx_t n = a_computed + 1; n <= N; ++n) {
        if (n < 6) {
            a_arr[n] = 0;
            continue;
        }
        idx_t limit = n - 6;
        seq_t sum = 0;
        for (idx_t t = 0; t <= limit; t += 2) {
            seq_t coeff = ((seq_t)(t / 4)) + 1;
            sum += coeff * p_arr[limit - t];
        }
        a_arr[n] = sum;
    }
    a_computed = N;
}

/* Public sequence function.  External linkage, no static keyword. */
seq_t a(idx_t n)
{
    if (n < 6) return 0;            /* values below offset are 0 */
    compute_a_sequence(n);          /* ensures a[n] is computed */
    return a_arr[n];
}
