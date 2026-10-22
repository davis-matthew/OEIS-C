/*
 * A316591  a(n) equals the coefficient of x^n in Sum_{m>=0} (x^m + 1 +
 *          1/x^m)^m for n >= 1.
 * https://oeis.org/A316591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Sum_{m>=0} (x^m + 1 + 1/x^m)^m
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *A   = NULL;   /* cache for a(1)..a(n) */
static idx_t  Alen = 0;     /* allocated length of A   */
static idx_t  Acmp = 0;     /* highest index computed */

/* Sum_{a} m!/(a! b! c!) where c=a+k, b=m-2a-k, 0≤a≤(m-k)/2   */
static seq_t coeff_sum(idx_t m, idx_t k) {
    if (k > m) return 0;          /* should not happen               */
    idx_t a_min = 0;
    idx_t a_max = (m - k) / 2;    /* floor division                 */
    seq_t sum = 0;
    for (idx_t a = a_min; a <= a_max; ++a) {
        idx_t c = a + k;
        seq_t coeff = binomial(m, a);
        coeff = coeff * binomial(m - a, c);
        sum += coeff;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* undefined for n = 0              */

    /* extend cache if necessary */
    if (Alen <= n) {
        idx_t newlen = Alen ? Alen << 1 : 4;
        if (newlen < n + 1) newlen = n + 1;
        seq_t *tmp = (seq_t *)realloc(A, newlen * sizeof(seq_t));
        if (!tmp) abort();
        A = tmp;
        Alen = newlen;
    }

    /* compute missing entries */
    if (Acmp < n) {
        for (idx_t i = Acmp + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    /* divisor m = d */
                    idx_t m  = d;
                    idx_t k  = i / m;
                    if (k <= m) sum += coeff_sum(m, k);

                    /* complementary divisor m2 = i/d */
                    if (d * d != i) {
                        idx_t m2 = i / d;        /* m2 ≥ d       */
                        idx_t k2 = d;            /* k2 = d ≤ m2 */
                        if (k2 <= m2) sum += coeff_sum(m2, k2);
                    }
                }
            }
            A[i] = sum;
        }
        Acmp = n;
    }
    return A[n];
}
