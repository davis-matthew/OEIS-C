/*
 * A363370  Number of ways to distribute n guards on the corners and walls of
 *          a square castle so that each wall has an equal number of guards
 *          modulo rotations and reflections.
 * https://oeis.org/A363370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^4+28*n^3+291*n^2+986*n+6480)/6912 * +
 *            (-1)^n*(5*n^2+70*n+48)/768 * + ((n+2) mod 3)*n/54 * +
 *            n*(((n+1)^3+1) mod 4)/32 ); * This expression may be combined
 *            over a common denominator of 6912. * For n ≥ 0 the result is an
 *            integer
 * Range:     u8 n <= 29, u16 n <= 138, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for constexpr literals */

seq_t a(idx_t n)
{
    /* Use unsigned long long and signed long long for the calculation,
     * which are large enough for n up to 10 000. */
    unsigned long long sn  = n;
    unsigned long long n2  = sn * sn;
    unsigned long long n3  = n2 * sn;
    unsigned long long n4  = n3 * sn;

    /* Numerator of the first rational term. */
    unsigned long long N1 = n4
                         + 28ULL * n3
                         + 291ULL * n2
                         + 986ULL * sn
                         + 6480ULL;

    /* Numerator of the second term: (-1)^n * (5*n^2+70*n+48)/768.
     * Amplify by 9 to bring to denominator 6912. */
    unsigned long long t  = 5ULL * n2 + 70ULL * sn + 48ULL;
    long long         N2 = ((n & 1ULL) ? -9LL * (long long)t : 9LL * (long long)t);

    /* Numerator of the third term: ((n+2) mod 3)*n/54.
     * Amplify by 128 to bring to denominator 6912. */
    unsigned long long N3 = ((sn + 2ULL) % 3ULL) * sn * 128ULL;

    /* Numerator of the fourth term: n*(((n+1)^3+1) mod 4)/32.
     * Amplify by 216 to bring to denominator 6912. */
    unsigned long long cube = (sn + 1ULL) * (sn + 1ULL) * (sn + 1ULL);
    unsigned long long mod4 = (cube + 1ULL) % 4ULL;
    unsigned long long N4 = sn * mod4 * 216ULL;

    /* Total numerator (may be negative due to N2). */
    long long N = (long long)N1 + N2 + (long long)N3 + (long long)N4;

    /* Integer division by the common denominator 6912. */
    seq_t result = (seq_t)(N / 6912ULL);
    return result;
}
