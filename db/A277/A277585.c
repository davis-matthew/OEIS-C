/*
 * A277585  Denominator of Sum_{k=0..n} (2^k * (k!)^2)/(2k + 1)!.
 * https://oeis.org/A277585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{k=0..n} (2^k * (k!)^2)/(2k + 1)!
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Euclidean algorithm for seq_t.  Works for built‑in types and for any
   seq_t that overloads % and / (e.g. boost::multiprecision::cpp_int). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Returned value is the denominator of the partial sum up to n. */
seq_t a(idx_t n)
{
    /* t_{k} = 1 + k/(2k+1) * t_{k+1},  t_{n+1} = 1.  We keep the fraction
       in lowest terms after each step.  The final denominator is t_{1}. */
    seq_t num = 1;   /* numerator of t_{k+1} */
    seq_t den = 1;   /* denominator of t_{k+1} */

    for (idx_t k = n; k > 0; --k)
    {
        seq_t kseq   = (seq_t)k;
        seq_t two_kp = (seq_t)(2 * k + 1);

        /* New denominator: (2k+1) * den */
        seq_t new_den = two_kp * den;

        /* New numerator: (2k+1) * den + k * num */
        seq_t new_num = two_kp * den + kseq * num;

        /* Reduce the fraction */
        seq_t g = seq_gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }

    return den;
}
