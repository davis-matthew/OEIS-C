/*
 * A258779  Expansion of (f(-x) * phi(x))^2 in powers of x where phi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A258779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 667, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A258779
    Formula: a(n) = coefficient of  
               ∏_{k≥1} (1 − x^k)^(−c_k)  with c_k periodic [2, −8, 2, −4].
    Equivalently a(n) = A000727(2*n); generated via the Euler transform
    of the period‑4 sequence [2, −8, 2, −4, …].
    Recurrence (a_0 = 1):
         a(n) = (1/n) * Σ_{k=1..n} r_k * a(n−k),
    where r_k = Σ_{d|k} d·c_d.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_a = NULL;   /* stored values a(0..max) */
static seq_t *seq_r = NULL;   /* auxiliary r_k values */
static size_t seq_max = 0;    /* number of allocated entries (max_n + 1) */

/* Ensure the arrays are computed up to index n (inclusive). */
static void ensure_size(idx_t n)
{
    size_t new_size = (size_t)n + 1;
    if (seq_max >= new_size) return;             /* already sufficient */

    /*--- 1. compute r_k up to new_size ---------------------------------*/
    seq_t *r = (seq_t*)calloc(new_size, sizeof(seq_t));
    for (size_t d = 1; d < new_size; ++d) {
        int mod = d % 4;
        int c   = (mod == 1 || mod == 3) ?  2
                 : (mod == 2           ? -8
                 :                              -4);
        seq_t val = (seq_t)d * (seq_t)c;
        for (size_t m = d; m < new_size; m += d)
            r[m] += val;
    }

    /*--- 2. compute a(n) up to new_size -------------------------------*/
    seq_t *a = (seq_t*)calloc(new_size, sizeof(seq_t));
    a[0] = 1;
    for (size_t i = 1; i < new_size; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= i; ++k)
            sum += r[k] * a[i - k];
        a[i] = sum / (seq_t)i;     /* guaranteed to be integer */
    }

    /*--- 3. replace old buffers ---------------------------------------*/
    free(seq_a);
    free(seq_r);
    seq_a   = a;
    seq_r   = r;
    seq_max = new_size;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_size(n);
    return seq_a[(size_t)n];
}
