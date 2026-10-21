/*
 * A290165  Reduced numerators of the Kirchhoff index for the n-halved cube
 *          graph.
 * https://oeis.org/A290165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, frac
 * Formula:   a(n) = reduced numerator of Kirchhoff index for the n‑halved
 *            cube graph. Kf = 2^(n‑1) · ∑_{a=1}^{⌊n/2⌋} C(n,a) / (2·a·(n‑a)),
 *            m(a) = C(n,a) (a<n/2), m(n/2) = C(n,n/2)/2 if n even. The result
 *            is the numerator of this fraction in lowest terms. Source:
 *            A290165 – Reduced numerators of the Kirchhoff i...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Add add_num/add_den to *num/*den and reduce. */
static void add_fraction(seq_t *num, seq_t *den, seq_t add_num, seq_t add_den)
{
    /* Reduce the addition by the GCD of current denominator and add_den. */
    seq_t g = gcd_seq_t(*den, add_den);
    seq_t fact1 = add_den / g;          /* factor for *num  */
    seq_t fact2 = *den / g;             /* factor for add_num */

    seq_t new_num = *num * fact1 + add_num * fact2;

    /* New denominator is lcm of the two denominators */
    seq_t new_den = *den / g * add_den;

    /* Reduce the result again */
    seq_t g2 = gcd_seq_t(new_num, new_den);
    *num = new_num / g2;
    *den = new_den / g2;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;  /* a(1) = 0 */

    idx_t half = n / 2;          /* floor(n/2) */
    seq_t sum_num = (seq_t)0;
    seq_t sum_den = (seq_t)1;

    /* cur = C(n,0) = 1 */
    seq_t cur = (seq_t)1;

    if (n % 2 == 0) {
        /* a from 1 to half-1 */
        for (idx_t a = 1; a < half; ++a) {
            cur = cur * (seq_t)(n - a + 1) / (seq_t)a;      /* C(n,a) */
            seq_t mult = cur;
            seq_t term_den = (seq_t)2 * (seq_t)a * (seq_t)(n - a);
            add_fraction(&sum_num, &sum_den, mult, term_den);
        }
        /* central term a = half */
        cur = cur * (seq_t)(n - half + 1) / (seq_t)half;     /* C(n,half) */
        seq_t mult = cur / (seq_t)2;                        /* divide by 2 */
        seq_t term_den = (seq_t)2 * (seq_t)half * (seq_t)(n - half);  /* n‑half = half */
        add_fraction(&sum_num, &sum_den, mult, term_den);
    } else {
        /* n odd: a from 1 to half (inclusive) */
        for (idx_t a = 1; a <= half; ++a) {
            cur = cur * (seq_t)(n - a + 1) / (seq_t)a;          /* C(n,a) */
            seq_t mult = cur;
            seq_t term_den = (seq_t)2 * (seq_t)a * (seq_t)(n - a);
            add_fraction(&sum_num, &sum_den, mult, term_den);
        }
    }

    /* multiply by V = 2^(n-1) */
    seq_t V = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        V = V * (seq_t)2;

    seq_t p = sum_num * V;   /* numerator of Kf fraction */
    seq_t q = sum_den;       /* denominator of Kf fraction */

    seq_t gfinal = gcd_seq_t(p, q);
    p = p / gfinal;          /* p and q are now coprime */

    return p;                 /* reduced numerator */
}
