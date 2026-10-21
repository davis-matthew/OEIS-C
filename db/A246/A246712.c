/*
 * A246712  Expansion of chi(x^2) / phi(x) in powers of x where phi(), chi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A246712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d * s(d) ) *
 *            a(n-k) where s(d) = { -2, 4, -2, 0, -2, 4, -2, 1 } for d ≡ 1…8
 *            (mod 8)
 * Range:     i8 n <= 7, i16 n <= 22, i32 n <= 72, i64 n <= 245, i128 n <= 876,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
using temp_t = typename std::conditional<(sizeof(seq_t) <= 8), __int128, seq_t>::type;
#else
typedef __int128 temp_t;
#endif

/* Period‑8 sequence s(d)   (s(1) = –2, s(2) = 4, … , s(8) = 1) */
static inline seq_t s_period8(idx_t d)
{
    switch (d & 7u) {            /* d mod 8 with unsigned operand            */
        case 0: return (seq_t)1;
        case 1: return (seq_t)(-2);
        case 2: return (seq_t)4;
        case 3: return (seq_t)(-2);
        case 4: return (seq_t)0;
        case 5: return (seq_t)(-2);
        case 6: return (seq_t)4;
        case 7: return (seq_t)(-2);
    }
    return (seq_t)0;              /* unreachable */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate space for coefficients up to n */
    size_t sz = (size_t)n + 1;                 /* fits in size_t for n ≤ 10000 */
    seq_t *a_vals = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *c_vals = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!a_vals || !c_vals) {                 /* allocation failure */
        free(a_vals);
        free(c_vals);
        return (seq_t)0;
    }

    a_vals[0] = (seq_t)1;                      /* a(0) = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        /* compute c(m) = Σ_{d|m} d * s(d) */
        temp_t cm = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                cm += (temp_t)d * (temp_t)s_period8(d);
                idx_t d2 = m / d;
                if (d2 != d)
                    cm += (temp_t)d2 * (temp_t)s_period8(d2);
            }
        }
        c_vals[m] = (seq_t)cm;

        /* compute a(m) = (1/m) * Σ_{k=1..m} c(k) * a(m-k) */
        temp_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += (temp_t)c_vals[k] * (temp_t)a_vals[m - k];
        }
        a_vals[m] = (seq_t)(sum / (temp_t)m);
    }

    seq_t result = a_vals[n];
    free(c_vals);
    free(a_vals);
    return result;
}
