/*
 * A349008  Area of the convex hull around R5 dragon curve expansion level n.
 * https://oeis.org/A349008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*5^(n-2) - 1 + \sum_{j=1}^{n-2}
 *            ((3*5^(n-2-j)-1)*HAgrow(2*b^j) + 2*5^(n-2-j)*HAgrow((4-i)*b^j)),
 *            b = 1+2i HAgrow(z) = min(|Re(z) + Re(z)|, |Im(z)|) after
 *            rotating z if sign(Re) ≠ sign(Im)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   sub_abs, sum_abs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sub_abs.h"
#include "../utils/sum_abs.h"

/* --------------------------------------------------------------------- */

/* Complex integer with signed parts ----------------------------------------- */
typedef struct {
    seq_t a;   /* |Re| */
    seq_t b;   /* |Im| */
    int   sa;  /* sign of Re: -1,0,1 */
    int   sb;  /* sign of Im: -1,0,1 */
} complexInt;

/* Multiply two complex integers ------------------------------------------------ */
static inline void complex_mul(const complexInt *p1, const complexInt *p2, complexInt *res)
{
    /* real = p1.re*p2.re - p1.im*p2.im */
    seq_t prod1 = p1->a * p2->a; int sprod1 = p1->sa * p2->sa;
    seq_t prod2 = p1->b * p2->b; int sprod2 = p1->sb * p2->sb;
    int  rsign; seq_t rabs;
    sub_abs(sprod1, prod1, sprod2, prod2, &rsign, &rabs);
    /* imag = p1.re*p2.im + p1.im*p2.re */
    seq_t prod3 = p1->a * p2->b; int s3 = p1->sa * p2->sb;
    seq_t prod4 = p1->b * p2->a; int s4 = p1->sb * p2->sa;
    int  isign; seq_t iabs;
    sum_abs(s3, prod3, s4, prod4, &isign, &iabs);

    res->a = rabs; res->sa = rsign;
    res->b = iabs; res->sb = isign;
}

/* Multiply complex integer by a non‑negative integer                      */
static inline void complex_mul_int(const complexInt *p, seq_t m, complexInt *res)
{
    res->a = p->a * m; res->sa = p->sa;
    res->b = p->b * m; res->sb = p->sb;
}

/* HAgrow: min(|2*Re(rot(z))|, |Im(rot(z))|)                                    */
static inline seq_t HAgrow(const complexInt *z)
{
    /* Rotate if signs differ */
    int sa = z->sa, sb = z->sb;
    seq_t a = z->a, b = z->b;
    if (!(sa == sb)) {
        /* rot(z) = i*z = -Im + i*Re */
        int new_sa = -sb;
        seq_t new_a = b;
        int new_sb = sa;
        seq_t new_b = a;
        sa = new_sa; sb = new_sb; a = new_a; b = new_b;
    }
    /* ShearRe: real becomes 2*re, imag stays same */
    seq_t real_abs = a * 2;
    int   real_sign = (a == 0) ? 0 : sa;
    seq_t imag_abs = b;
    int   imag_sign = (b == 0) ? 0 : sb;
    /* HAgrow is the smaller of |2*Re| and |Im| */
    return (real_abs < imag_abs) ? real_abs : imag_abs;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    /* 5^(n-2) */
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n - 2; ++i) pow5 *= 5;

    seq_t result = 17 * pow5 - 1;

    /* b = 1 + 2i */
    complexInt b = { (seq_t)1, (seq_t)2, 1, 1 };

    /* current power of b, starting at b^1 */
    complexInt cur = b;

    /* pow5_term = 5^(n-3) (for j=1), if n >= 3 */
    seq_t pow5_term = 0;
    if (n >= 3) pow5_term = pow5 / 5;

    for (idx_t j = 1; j <= n - 2; ++j) {
        /* HAgrow(2 * cur) */
        complexInt temp1;
        complex_mul_int(&cur, (seq_t)2, &temp1);
        seq_t h1 = HAgrow(&temp1);

        /* HAgrow((4-i) * cur) */
        complexInt coeff = { (seq_t)4, (seq_t)1, 1, -1 };   /* (4 - i) */
        complexInt temp2;
        complex_mul(&coeff, &cur, &temp2);
        seq_t h2 = HAgrow(&temp2);

        /* coeff1 = 3*pow5_term - 1, coeff2 = 2*pow5_term */
        seq_t coeff1 = 3 * pow5_term - 1;
        seq_t coeff2 = 2 * pow5_term;

        result += coeff1 * h1 + coeff2 * h2;

        /* advance cur to next power of b */
        complexInt next;
        complex_mul(&cur, &b, &next);
        cur = next;

        /* next pow5_term if needed */
        if (j < n - 2) pow5_term /= 5;
    }

    return result;
}
