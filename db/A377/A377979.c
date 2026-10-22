/*
 * A377979  List of exponents in the expansion of (1 - q)*Sum_{n >= 0}
 *          q^(2*n*(n+1))*Product_{k >= 2*n+1} 1 - q^k.
 * https://oeis.org/A377979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = piecewise quadratics according to n mod 8: * n = 1 → 0 *
 *            n ≡ 1 (mod 8): 12*k² + 5*k + 1 (k = (n-1)/8) * n ≡ 2 (mod 8):
 *            12*k² + 7*k + 1 * n ≡ 3 (mod 8): 12*k² + 11*k + 3 * n ≡ 4 (mod
 *            8): 12*k² + 13*k + 4 * n ≡ 5 (mod 8): 12*k² + 17*k + 6 * n ≡ 6
 *            (mod 8): 12*k² + 19*k + 8 * n ≡ 7 (mod 8): 12...
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    idx_t r = n % 8;
    seq_t k;
    if (r == 0)
        k = ((seq_t)n - 8) / 8;
    else
        k = ((seq_t)n - r) / 8;

    switch (r) {
        case 1: return 12 * k * k + 5 * k + 1;
        case 2: return 12 * k * k + 7 * k + 1;
        case 3: return 12 * k * k + 11 * k + 3;
        case 4: return 12 * k * k + 13 * k + 4;
        case 5: return 12 * k * k + 17 * k + 6;
        case 6: return 12 * k * k + 19 * k + 8;
        case 7: return 12 * k * k + 23 * k + 11;
        case 0: return 12 * k * k + 25 * k + 13;
    }
    return 0; /* unreachable */
}
