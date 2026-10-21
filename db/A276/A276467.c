/*
 * A276467  a(n) = denominator of Sum_{d|n} 0.d.
 * https://oeis.org/A276467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276467:  a(n) = denominator of ∑_{d|n} 0.d
    Exactly as defined: 0.d is d divided by 10^{digits(d)}.
    The implementation finds the maximum number of decimal digits
    among all divisors of n, builds the common denominator 10^max,
    adds the scaled numerator contributions, and finally reduces
    by the greatest common divisor to obtain the output denominator.    */

#include <stdlib.h>

/* power of ten: 10^e   (seq_t is already defined by the build) */
static seq_t seq_pow10(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* number of decimal digits of an idx_t value (at least one digit) */
static int digits_idx(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Euclidean algorithm for seq_t gcd */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Sequence A276467: denominator of ∑_{d|n} 0.d   (exact integer)    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* not used, but harmless */

    /* ---- Find the maximum number of decimal digits among all divisors ---- */
    int maxdigits = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            int d1 = digits_idx(i);
            if (d1 > maxdigits) maxdigits = d1;
            idx_t j = n / i;
            if (j != i) {
                int d2 = digits_idx(j);
                if (d2 > maxdigits) maxdigits = d2;
            }
        }
    }

    /* Common denominator 10^maxdigits */
    seq_t denom = seq_pow10((idx_t)maxdigits);

    /* ---- Compute the numerator over this common denominator ---- */
    seq_t numer = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            int d1 = digits_idx(i);
            seq_t mult1 = seq_pow10((idx_t)(maxdigits - d1));
            numer += (seq_t)i * mult1;

            idx_t j = n / i;
            if (j != i) {
                int d2 = digits_idx(j);
                seq_t mult2 = seq_pow10((idx_t)(maxdigits - d2));
                numer += (seq_t)j * mult2;
            }
        }
    }

    /* ---- Reduce the fraction numer/denom ---- */
    seq_t g = gcd_seq(numer, denom);
    return denom / g;
}
