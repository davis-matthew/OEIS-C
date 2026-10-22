/*
 * A325157  G.f.: A(x) = Sum_{n>=0} x^n * (1 + (-1)^n * A(x))^n / (1 - (-1)^n
 *          * x*A(x))^(n+1).
 * https://oeis.org/A325157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence derived from the algebraic functional equation 4x^2
 *            A(x)^3 – (1–x)^2 A(x) + (1–x) = 0. a(0)=1, a(1)=1, for n ≥ 2:
 *            a(n) = 4 * Σ_{i+j+k=n-2} a(i) a(j) a(k) + 2 a(n-1) – a(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_arr   = NULL;   /* sequence values */
static size_t a_cap   = 0;       /* allocated size of a_arr   */
static size_t a_len   = 0;       /* number of elements stored */

static seq_t *b_arr   = NULL;   /* convolution a*a, indices 0 … 2*max_n */
static size_t b_cap   = 0;       /* allocated size of b_arr */

/* Ensure that a_arr can hold indices up to n. */
static void ensure_a_cap(idx_t n)
{
    if ((size_t)n < a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 8;
    while ((size_t)n >= new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    a_arr = tmp;
    /* new slots are uninitialized; they will be written before use */
    a_cap = new_cap;
}

/* Ensure that b_arr can hold indices up to 2*n. */
static void ensure_b_cap(idx_t k)
{
    size_t needed = (size_t)(k * 2 + 1);
    if (needed <= b_cap) return;
    size_t new_cap = b_cap ? b_cap << 1 : 8;
    while (needed > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    /* zero any newly allocated memory */
    if (new_cap > b_cap)
        memset(tmp + b_cap, 0, (new_cap - b_cap) * sizeof(seq_t));
    b_arr = tmp;
    b_cap = new_cap;
}

seq_t a(idx_t n)
{
    if ((size_t)n >= a_cap)  /* grow storage if necessary */
        ensure_a_cap(n);

    /* Compute values up to n if needed */
    if ((size_t)n >= a_len) {
        for (idx_t k = a_len; k <= n; ++k) {
            /* Compute a(k) */
            if (k == 0) {
                a_arr[0] = 1;
            } else if (k == 1) {
                a_arr[1] = 1;
            } else {
                seq_t S = 0;
                /* S = Σ_{i=0}^{k-2} a(i) * b[k-2-i] */
                for (idx_t i = 0; i <= k - 2; ++i)
                    S += a_arr[i] * b_arr[k - 2 - i];
                S = S * (seq_t)4;
                /* a(k) = 4*S + 2*a(k-1) - a(k-2) */
                seq_t val = S + (a_arr[k - 1] + a_arr[k - 1]) - a_arr[k - 2];
                a_arr[k] = val;
            }

            /* Update convolution b = a * a */
            ensure_b_cap(k);
            seq_t ak = a_arr[k];
            for (idx_t i = 0; i < k; ++i) {
                seq_t prod = a_arr[i] * ak;
                seq_t twice = prod + prod;      /* 2 * prod */
                b_arr[i + k] += twice;
            }
            seq_t prod = ak * ak;
            b_arr[2 * k] += prod;

            ++a_len;
        }
    }
    return a_arr[n];
}
