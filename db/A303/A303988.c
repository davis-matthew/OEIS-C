/*
 * A303988  Triangle read by rows: numerators of c_{n,k}, n >= 0, 0 <= k <= n,
 *          used in the proof that Zeta(3) is irrational.
 * https://oeis.org/A303988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = numerator T(r,k) of c_{r,k} from OEIS A303988, where r is
 *            the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n
 *            – r*(r+1)/2. T(r,k) is defined by c_{r,k} = Zeta3(r) +
 *            Σ_{m=1..k} (-1)^{m-1} / (2*m³*B(r,m)), with B(r,m) =
 *            binom(r+m,2m)*binom(2m,m) and Zeta3(r) = Σ_{t=1..r} 1/t³ in...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

/* Compute Zeta3(r) as a reduced fraction num/den = Σ_{t=1..r} 1/t³. */
static void
zeta3_fraction(idx_t r, seq_t *num, seq_t *den)
{
    seq_t n = 0;
    seq_t d = 1;

    for (idx_t t = 1; t <= r; ++t) {
        seq_t tt = seq_mul((seq_t)t, (seq_t)t);          /* t²        */
        seq_t t3 = seq_mul(tt, (seq_t)t);                /* t³        */
        seq_t new_num = seq_mul(n, t3) + d;              /* n*t³ + d  */
        seq_t new_den = seq_mul(d, t3);                  /* d*t³      */
        seq_t g = gcd(new_num, new_den);
        n = new_num / g;
        d = new_den / g;
    }

    *num = n;
    *den = d;
}

/* Return the flattened triangle value a(n). */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;          /* column index in row r   */

    seq_t num, den;
    if (r == 0) {                      /* Zeta3(0) = 0/1            */
        num = 0;
        den = 1;
    } else {
        zeta3_fraction(r, &num, &den);
    }

    /* Add the terms for m = 1 .. k. */
    for (idx_t m = 1; m <= k; ++m) {
        seq_t m_s = (seq_t)m;
        seq_t m2 = seq_mul(m_s, m_s);          /* m²  */
        seq_t m3 = seq_mul(m2, m_s);           /* m³  */

        seq_t bin1 = binom(r + m, 2 * m);
        seq_t bin2 = binom(2 * m, m);
        seq_t B    = seq_mul(bin1, bin2);

        seq_t den_term = seq_mul(seq_mul((seq_t)2, m3), B);  /* 2*m³*B */

        seq_t new_num = seq_mul(num, den_term);
        if (m & 1)               /* odd m  → add 1/den_term */
            new_num = new_num + den;
        else                     /* even m → subtract       */
            new_num = new_num - den;

        seq_t new_den = seq_mul(den, den_term);

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
