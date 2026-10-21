/*
 * A257464  Number of factorizations of m^n into 3 factors, where m is a
 *          product of exactly 3 distinct primes and each factor is a product
 *          of n primes (counted with multiplicity).
 * https://oeis.org/A257464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered factorizations of mⁿ into 3 factors,
 *            where m = p·q·r is the product of three distinct primes and each
 *            factor contains exactly n primes (counted with multiplicity).
 *            Derived closed-form expression (valid for all n ≥ 0): Let N = n.
 *            Let F2 = # of integer triples (x,y,z) wit...
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 272, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t for arithmetic */
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;           /* N^2 */
    seq_t N3  = N2 * N;          /* N^3 */
    seq_t N4  = N2 * N2;         /* N^4 */

    /* base polynomial: N^4 + 6N^3 + 15N^2 + 18N + 8 */
    seq_t base = N4 + (seq_t)6 * N3 + (seq_t)15 * N2 + (seq_t)18 * N + (seq_t)8;

    /* compute F2 */
    seq_t F2;
    if ((N & (seq_t)1) == 0)          /* N even */
    {
        seq_t m = N >> 1;             /* m = N/2 */
        F2 = ((m + (seq_t)1) * (m + (seq_t)2)) >> 1;   /* divide by 2 */
    }
    else                              /* N odd */
    {
        seq_t m = N >> 1;             /* m = floor(N/2) */
        F2 = ((seq_t)m * (m + (seq_t)1)) >> 1;
    }

    /* compute F3 */
    seq_t F3 = (N % (seq_t)3 == 0) ? (seq_t)1 : (seq_t)0;

    /* numerator: base + 24*F2 + 16*F3 */
    seq_t num = base + (seq_t)24 * F2 + (seq_t)16 * F3;

    /* final division by 48 */
    return num / (seq_t)48;
}
