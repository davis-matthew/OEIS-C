/*
 * A397966  Number of points with even coordinate sum in the 5-dimensional
 *          hypercube [-n,n]^5 having an odd number of lattice neighbors
 *          inside the hypercube.
 * https://oeis.org/A397966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*n^4 - 160*n^3 + 280*n^2 - 200*n + 61 + 61*(-1)^n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 86, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;
    seq_t base = 80 * n4 - 160 * n3 + 280 * n2 - 200 * ns + 61;
    return ((n & 1) == 0) ? (base + 61) : (base - 61);
}
