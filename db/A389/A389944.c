/*
 * A389944  G.f. A(x) satisfies A(x) = 1 + x*(1-x^3)^2*A(x)^3.
 * https://oeis.org/A389944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)}
 *            (-1)^k*binomial(2*(n-3*k),k)*A001764(n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from A(x)=1 + x*(1-x^3)^2*A(x)^3:
   a(0)=1
   a(n) = c(n-1) - 2*c(n-4) + c(n-7)   for n≥1
   where c(m) = Σ_{i=0..m} a[i]*s[m-i]
   and s(m) = Σ_{j=0..m} a[j]*a[m-j]
   (s(m) is the convolution a*a). */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_a = NULL;
static seq_t *seq_s = NULL;
static idx_t  seq_cap   = 0;
static idx_t  seq_last  = 0;          /* last index for which a[] and s[] are computed */

/* Ensure that a[0..n] and s[0..n] are available */
static void ensure_cache(idx_t n)
{
    if (n <= seq_last) return;                    /* already computed */

    /* enlarge storage if necessary */
    if (seq_cap < n + 1) {
        idx_t new_cap = seq_cap ? seq_cap << 1 : (n + 1);
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *a_new = (seq_t*)realloc(seq_a, new_cap * sizeof(seq_t));
        seq_t *s_new = (seq_t*)realloc(seq_s, new_cap * sizeof(seq_t));
        if (!a_new || !s_new) exit(EXIT_FAILURE);
        for (idx_t i = seq_cap; i < new_cap; ++i) {
            a_new[i] = 0;
            s_new[i] = 0;
        }
        seq_a = a_new;
        seq_s = s_new;
        seq_cap = new_cap;
    }

    /* initialise a[0] and s[0] once */
    if (seq_last == 0) {
        seq_a[0] = 1;
        seq_s[0] = 1;            /* a[0]*a[0] */
        seq_last = 0;
    }

    /* compute values from seq_last+1 up to n */
    for (idx_t k = seq_last + 1; k <= n; ++k) {
        /* c(k-1) */
        seq_t c1 = 0;
        for (idx_t i = 0; i <= k - 1; ++i)
            c1 += seq_a[i] * seq_s[(k - 1) - i];

        /* c(k-4) */
        seq_t c2 = 0;
        if (k >= 4) {
            for (idx_t i = 0; i <= k - 4; ++i)
                c2 += seq_a[i] * seq_s[(k - 4) - i];
        }

        /* c(k-7) */
        seq_t c3 = 0;
        if (k >= 7) {
            for (idx_t i = 0; i <= k - 7; ++i)
                c3 += seq_a[i] * seq_s[(k - 7) - i];
        }

        /* a(k) = c1 - 2*c2 + c3 */
        seq_t val = c1;
        if (k >= 4) {
            seq_t two_c2 = c2 + c2;
            val -= two_c2;
        }
        if (k >= 7) {
            val += c3;
        }
        seq_a[k] = val;

        /* s(k) = Σ_{j=0..k} a[j]*a[k-j] */
        seq_t sum = 0;
        for (idx_t j = 0; j <= k; ++j)
            sum += seq_a[j] * seq_a[k - j];
        seq_s[k] = sum;
    }

    seq_last = n;
}

/* Public function as required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* a(0)=1 */
    ensure_cache(n);
    return seq_a[n];
}
