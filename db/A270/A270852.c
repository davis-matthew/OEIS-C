/*
 * A270852  Number of 3X3X3 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1, n or n-1 exactly once.
 * https://oeis.org/A270852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(2) = 0 a(3) = 30 a(4) = 270 a(5) = 1650 a(6) = 7344
 *            a(7) = 24954 For n > 7: a(n) = 27*n^5 – 531*n^4 + 4794*n^3 –
 *            24417*n^2 + 69189*n – 86274 (even n) a(n) = 27*n^5 – 531*n^4 +
 *            4806*n^3 – 24624*n^2 + 70425*n – 88833 (odd n) The two quintic
 *            formulas give the exact values for all n > 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for 1 ≤ n ≤ 7 */
    static const seq_t base[7] = {
        0,      /* a(1) */
        0,      /* a(2) */
        30,     /* a(3) */
        270,    /* a(4) */
        1650,   /* a(5) */
        7344,   /* a(6) */
        24954   /* a(7) */
    };

    if (n <= 7)
        return base[n - 1];

    /* For n > 7 use the appropriate quintic polynomial.  We compute
       the result in a signed 128‑bit integer to avoid intermediate
       underflow when using the subtraction terms.  The final value
       is guaranteed non‑negative and is cast back to seq_t.  */
    __int128 val = 0;
    unsigned long long n1 = (unsigned long long)n;        /* n */
    unsigned long long n2 = n1 * n1;                      /* n² */
    unsigned long long n3 = n2 * n1;                      /* n³ */
    unsigned long long n4 = n2 * n2;                      /* n⁴ */
    unsigned long long n5 = n4 * n1;                      /* n⁵ */

    if ((n & 1ULL) == 0) { /* even n */
        val = (__int128)27   * n5
            - (__int128)531  * n4
            + (__int128)4794 * n3
            - (__int128)24417* n2
            + (__int128)69189* n1
            - (__int128)86274;
    } else {                /* odd n */
        val = (__int128)27   * n5
            - (__int128)531  * n4
            + (__int128)4806 * n3
            - (__int128)24624* n2
            + (__int128)70425* n1
            - (__int128)88833;
    }

    return (seq_t)val;
}
