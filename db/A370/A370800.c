/*
 * A370800  Expansion of (1/x) * Series_Reversion( x/(x+1/(1-x+x^3)) ).
 * https://oeis.org/A370800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(n,k) * b(k),
 * where b(0)=1 and for n>=1
 * b(n) = sum_{i=0}^{n-1} b(i)*b(n-1-i) - sum_{i=0}^{n-3} c(i)*c(n-3-i),
 * with c(m) = sum_{i=0}^{m} b(i)*b(m-i).
 *     B(x) = (1/x) * Series_Reversion( x*(1-x+x^3) ) ,
 * and a(0)=1, a(1)=2, a(2)=5, ...
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *b = NULL;        /* coefficients of b(k)   */
static seq_t *c = NULL;        /* auxiliary coefficients   */
static idx_t b_count = 0;      /* number of b values stored   */
static idx_t c_count = 0;      /* number of c values stored   */

/* Ensure that b[0..n] and c[0..n] are computed and arrays are large enough. */
static void ensure_b(idx_t n)
{
    /* First allocation and initialization. */
    if (b_count == 0) {
        size_t cap = n + 1;
        if (cap < 10) cap = 10;           /* some initial spare capacity          */
        b = (seq_t *)malloc(sizeof(seq_t) * cap);
        c = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!b || !c) exit(1);
        b[0] = 1;
        c[0] = 1;
        b_count = 1;
        c_count = 1;
    }

    /* Enlarge capacity if needed. */
    if (n + 1 > b_count) {
        size_t newcap = (b_count == 0) ? (n + 1) : (b_count * 2);
        if (newcap < n + 1) newcap = n + 1;
        seq_t *b_new = (seq_t *)realloc(b, sizeof(seq_t) * newcap);
        seq_t *c_new = (seq_t *)realloc(c, sizeof(seq_t) * newcap);
        if (!b_new || !c_new) exit(1);
        b = b_new;
        c = c_new;
        /* b_count and c_count keep current number of elements; capacity now newcap */
    }

    /* Compute new entries up to n. */
    for (idx_t idx = b_count; idx <= n; ++idx) {
        /* sigma1 = sum_{i=0}^{idx-1} b[i]*b[idx-1-i] */
        seq_t sigma1 = 0;
        for (idx_t i = 0; i < idx; ++i)
            sigma1 += b[i] * b[idx - 1 - i];

        /* sigma2 = sum_{i=0}^{idx-3} c[i]*c[idx-3-i]  (only if idx>=3) */
        seq_t sigma2 = 0;
        if (idx >= 3) {
            for (idx_t i = 0; i <= idx - 3; ++i)
                sigma2 += c[i] * c[idx - 3 - i];
        }

        /* b[idx] = sigma1 - sigma2 */
        b[idx] = sigma1 - sigma2;

        /* c[idx] = sum_{i=0}^{idx} b[i]*b[idx-i] */
        seq_t sumc = 0;
        for (idx_t i = 0; i <= idx; ++i)
            sumc += b[i] * b[idx - i];
        c[idx] = sumc;
    }
    b_count = n + 1;
    c_count = n + 1;
}

seq_t a(idx_t n)
{
    ensure_b(n);

    /* Compute binomial transform: sum_{k=0}^{n} C(n,k) * b[k] */
    seq_t sum = 0;
    seq_t binom = 1;                    /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        sum += binom * b[k];
        if (k < n) {
            seq_t mult = (seq_t)(n - k);
            seq_t div  = (seq_t)(k + 1);
            binom = binom * mult / div;
        }
    }
    return sum;
}
