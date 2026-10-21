/*
 * A288873  Numerators of scaled Bernoulli numbers 4^n*B(n), with B(n) =
 *          A027641(n)/A027642(n).
 * https://oeis.org/A288873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator(4^n * B(n)) where B(n) are Bernoulli numbers
 *            defined by Σ_{k=0}^{n} binom(n+1,k) B(k) = 0 with B(0)=1,
 *            B(1)=-1/2. The function computes this numerator for any n ≥ 0
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 11, i64 n <= 23, i128 n <= 30,
 *            big any n
 * Helpers:   free_seq_array, gcd, pow2, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *Ber_num = NULL;  /* numerator of B(n) */
static seq_t *Ber_den = NULL;  /* denominator of B(n) */
static seq_t *A_num   = NULL;  /* temporary array in the algorithm */
static seq_t *A_den   = NULL;
static idx_t  capacity        = 0;
static idx_t  max_n_calculated = 0;
static bool   initialized      = false;

/* absolute value for seq_t */
static inline seq_t abs_seq(seq_t x) { return x < 0 ? -x : x; }

/* compute Bernoulli numbers up to n, storing in Ber_num/Ber_den */
static void compute_up_to(idx_t n)
{
    if (!initialized) {
        capacity = n + 1;
        Ber_num = seq_alloc_new_array(capacity);
        Ber_den = seq_alloc_new_array(capacity);
        A_num   = seq_alloc_new_array(capacity);
        A_den   = seq_alloc_new_array(capacity);

        /* B(0) = 1 */
        Ber_num[0] = (seq_t)1;
        Ber_den[0] = (seq_t)1;
        A_num[0]   = (seq_t)1;
        A_den[0]   = (seq_t)1;

        max_n_calculated = 0;
        initialized = true;
    }

    if (n <= max_n_calculated) return;

    if (capacity < n + 1) {
        /* enlarge all arrays */
        size_t new_cap = n + 1;
        seq_t *newBer_num = seq_alloc_new_array(new_cap);
        seq_t *newBer_den = seq_alloc_new_array(new_cap);
        seq_t *newA_num   = seq_alloc_new_array(new_cap);
        seq_t *newA_den   = seq_alloc_new_array(new_cap);

        for (idx_t i = 0; i <= max_n_calculated; ++i) {
            newBer_num[i] = Ber_num[i];
            newBer_den[i] = Ber_den[i];
            newA_num[i]   = A_num[i];
            newA_den[i]   = A_den[i];
        }

        free_seq_array(Ber_num);
        free_seq_array(Ber_den);
        free_seq_array(A_num);
        free_seq_array(A_den);
        Ber_num = newBer_num;
        Ber_den = newBer_den;
        A_num   = newA_num;
        A_den   = newA_den;
        capacity = new_cap;
    }

    /* continue the Akiyama–Tanigawa algorithm from the last computed index */
    for (idx_t m = max_n_calculated + 1; m <= n; ++m) {
        A_num[m] = (seq_t)1;
        A_den[m] = (seq_t)(m + 1);

        for (idx_t j = m; j > 0; --j) {
            /* diff = a[j-1] - a[j] */
            seq_t diff_num = A_num[j-1] * A_den[j] - A_num[j] * A_den[j-1];
            seq_t diff_den = A_den[j-1] * A_den[j];
            diff_num = diff_num * (seq_t)j;          /* multiply by j */

            seq_t g = gcd(abs_seq(diff_num), diff_den);
            diff_num = diff_num / g;
            diff_den = diff_den / g;

            A_num[j-1] = diff_num;
            A_den[j-1] = diff_den;
        }
        /* after the inner loop a[0] stores B(m) */
        Ber_num[m] = A_num[0];
        Ber_den[m] = A_den[0];
    }

    max_n_calculated = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n & 1U) return (seq_t)0;   /* odd n > 1 */

    compute_up_to(n);

    seq_t num = Ber_num[n];
    seq_t den = Ber_den[n];

    seq_t pow2_val = pow2((idx_t)(2 * n));
    seq_t g1       = gcd(den, pow2_val);
    seq_t pow2_div = pow2_val / g1;
    seq_t result   = num * pow2_div;   /* numerator of 4^n * B(n) */

    return result;
}
