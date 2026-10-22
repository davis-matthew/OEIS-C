/*
 * A370472  G.f. satisfies A(x) = 1 + x * A(x) * (1 - A(x) + A(x)^2 - A(x)^3 +
 *          A(x)^4).
 * https://oeis.org/A370472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A370472:  G.f. A(x) = 1 + x A(x) (1 – A + A^2 – A^3 + A^4)
   Recurrence for the coefficients:
      a(0) = 1
      a(n) = Σ_{i=0}^{n‑1} a(i) · b(n‑1‑i)   (n ≥ 1)
   where b is given by
      b(0) = 1
      b(k) = –a(k) + c2(k) – c3(k) + c4(k)   (k ≥ 1)
   with
      c2(k) = Σ_{i=0}^{k}   a(i) a(k‑i)
      c3(k) = Σ_{i=0}^{k}   a(i) c2(k‑i)
      c4(k) = Σ_{i=0}^{k}   a(i) c3(k‑i)
*/

#include <stdlib.h>

/* External integer types are defined by the build system. */
extern seq_t a(idx_t n);          /* declared here for completeness */

/* Internal static arrays for memoization.  They are grown only upwards. */
static seq_t *a_arr   = NULL;
static seq_t *b_arr   = NULL;
static seq_t *c2_arr  = NULL;
static seq_t *c3_arr  = NULL;
static seq_t *c4_arr  = NULL;
static idx_t  cache_n = 0;        /* highest index that has been computed */

/* Helper function that ensures all arrays are large enough for index n. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_n) return;          /* already have enough capacity */

    /* Determine new size.  The +1 accounts for the zero index. */
    size_t new_size = (size_t)(n + 1);

    /* Reallocate all arrays. */
    a_arr  = (seq_t*)realloc(a_arr,  new_size * sizeof(seq_t));
    b_arr  = (seq_t*)realloc(b_arr,  new_size * sizeof(seq_t));
    c2_arr = (seq_t*)realloc(c2_arr, new_size * sizeof(seq_t));
    c3_arr = (seq_t*)realloc(c3_arr, new_size * sizeof(seq_t));
    c4_arr = (seq_t*)realloc(c4_arr, new_size * sizeof(seq_t));

    /* If this is the first allocation, initialise the base case. */
    if (cache_n == 0) {
        a_arr[0]  = (seq_t)1;
        b_arr[0]  = (seq_t)1;              /* constant term 1 */
        c2_arr[0] = a_arr[0] * a_arr[0];   /* a0^2 = 1 */
        c3_arr[0] = a_arr[0] * c2_arr[0];  /* a0^3 = 1 */
        c4_arr[0] = a_arr[0] * c3_arr[0];  /* a0^4 = 1 */
        cache_n  = 0;
    }

    /* Compute new terms up to n. */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        /* a(k) = Σ_{i=0}^{k-1} a[i] * b[k-1-i] */
        seq_t sum_a = 0;
        for (idx_t i = 0; i < k; ++i) {
            sum_a += a_arr[i] * b_arr[k - 1 - i];
        }
        a_arr[k] = sum_a;

        /* c2(k) = Σ_{i=0}^{k} a[i] * a[k-i] */
        seq_t sum_c2 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum_c2 += a_arr[i] * a_arr[k - i];
        }
        c2_arr[k] = sum_c2;

        /* c3(k) = Σ_{i=0}^{k} a[i] * c2(k-i) */
        seq_t sum_c3 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum_c3 += a_arr[i] * c2_arr[k - i];
        }
        c3_arr[k] = sum_c3;

        /* c4(k) = Σ_{i=0}^{k} a[i] * c3(k-i) */
        seq_t sum_c4 = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum_c4 += a_arr[i] * c3_arr[k - i];
        }
        c4_arr[k] = sum_c4;

        /* b(k) = –a(k) + c2(k) – c3(k) + c4(k)
           Compute as (c2 + c4) – (a + c3) to avoid unsigned underflow. */
        seq_t tmp1 = c2_arr[k] + c4_arr[k];
        seq_t tmp2 = a_arr[k] + c3_arr[k];
        b_arr[k]   = tmp1 - tmp2;
    }

    cache_n = n;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    /* Ensure we have computed up to n. */
    ensure_capacity(n);
    return a_arr[n];
}
