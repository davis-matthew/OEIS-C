/*
 * A250725  Number of (n+1) X (4+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 114 For n > 1, let npar = n & 1. a(n) = (2*n⁵ + 65*n⁴ +
 *            320*n³ + 2035*n² + 7718*n + (npar?3660:3600)) / 120 (All
 *            divisions are exact.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 87, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)114;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;               // n²
    seq_t n3 = n2 * nn;               // n³
    seq_t n4 = n3 * nn;               // n⁴
    seq_t n5 = n4 * nn;               // n⁵

    seq_t num = 2 * n5             +   // 2·n⁵
                65 * n4            +   // 65·n⁴
                320 * n3           +   // 320·n³
                2035 * n2           +   // 2035·n²
                7718 * nn            +   // 7718·n
                ((n & 1) ? 3660 : 3600); // parity term

    return num / (seq_t)120;
}
