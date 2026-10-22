/*
 * A317133  G.f.: Sum_{n>=0} binomial(4*(n+1), n)/(n+1) * x^n / (1+x)^(n+1).
 * https://oeis.org/A317133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317133
   a(0) = 1
   a(n) = 3*a(n-1)
          + 6*conv2[n-2] (if n>=2)
          + 4*conv3[n-3] (if n>=3)
          +    conv4[n-4] (if n>=4)
   where
     conv2[i] = sum_{k=0..i} a[k]*a[i-k]
     conv3[i] = sum_{k=0..i} a[k]*conv2[i-k]
     conv4[i] = sum_{k=0..i} a[k]*conv3[i-k]
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t _max_n = 0;
static seq_t *A = NULL;
static seq_t *Conv2 = NULL;
static seq_t *Conv3 = NULL;
static seq_t *Conv4 = NULL;

/* ensure the internal arrays are large enough to hold indices up to n */
static void _ensure_capacity(idx_t n)
{
    if (n < _max_n) return;
    idx_t new_size = n + 1;
    A =     (seq_t*)realloc(A,     new_size * sizeof(seq_t));
    Conv2 = (seq_t*)realloc(Conv2, new_size * sizeof(seq_t));
    Conv3 = (seq_t*)realloc(Conv3, new_size * sizeof(seq_t));
    Conv4 = (seq_t*)realloc(Conv4, new_size * sizeof(seq_t));
    if (A == NULL || Conv2 == NULL || Conv3 == NULL || Conv4 == NULL) {
        /* allocation failed – abort */
        exit(1);
    }

    /* compute values from the old size up to the new size */
    for (idx_t i = _max_n; i <= n; ++i) {
        seq_t val = 0;
        if (i == 0) {
            val = 1;
        } else {
            val = 3 * A[i-1];
            if (i >= 2) val += 6 * Conv2[i-2];
            if (i >= 3) val += 4 * Conv3[i-3];
            if (i >= 4) val +=      Conv4[i-4];
        }
        A[i] = val;

        /* Conv2[i] = sum_{k=0..i} a[k] * a[i-k] */
        seq_t c2 = 0;
        for (idx_t k = 0; k <= i; ++k) {
            c2 += A[k] * A[i-k];
        }
        Conv2[i] = c2;

        /* Conv3[i] = sum_{k=0..i} a[k] * Conv2[i-k] */
        seq_t c3 = 0;
        for (idx_t k = 0; k <= i; ++k) {
            c3 += A[k] * Conv2[i-k];
        }
        Conv3[i] = c3;

        /* Conv4[i] = sum_{k=0..i} a[k] * Conv3[i-k] */
        seq_t c4 = 0;
        for (idx_t k = 0; k <= i; ++k) {
            c4 += A[k] * Conv3[i-k];
        }
        Conv4[i] = c4;
    }
    _max_n = n;
}

seq_t a(idx_t n)
{
    _ensure_capacity(n);
    return A[n];
}
