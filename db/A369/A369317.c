/*
 * A369317  a(n) = A091255(n, n + 1).
 * https://oeis.org/A369317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A091255(n, n+1) // gcd of binary polynomials of n and n+1
 *            over GF(2)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------
  Helper: bit length of a value in the native seq_t type.
  Works for fixed‐width integers, unsigned __int128, and
  arbitrary–precision integer classes that support >> and
  != comparison with 0 in C++.
  --------------------------------------------------------------*/
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x != (seq_t)0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/*--------------------------------------------------------------
  Polynomial remainder of a over b in GF(2).  Both a and b are
  represented by their binary coefficients as unsigned integers.
  --------------------------------------------------------------*/
static seq_t poly_mod_gf2(seq_t a, seq_t b)
{
    /* loop while the divisor is non‑zero and the dividend has
       degree ≥ divisor's degree.  All operations are XOR (±),
       shifts, and comparisons. */
    while (b != (seq_t)0 && a != (seq_t)0) {
        unsigned degA = bitlen_seq_t(a) - 1;
        unsigned degB = bitlen_seq_t(b) - 1;
        if (degA < degB) {
            break;
        }
        unsigned shift = degA - degB;
        a ^= (b << shift);
    }
    return a;
}

/*--------------------------------------------------------------
  Euclidean algorithm for polynomials over GF(2).
  --------------------------------------------------------------*/
static seq_t poly_gcd_gf2(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = poly_mod_gf2(a, b);
        a = b;
        b = r;
    }
    return a;
}

/*--------------------------------------------------------------
  Main sequence function: a(n) = A091255(n, n+1).
  --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t m_t = n_t + (seq_t)1;      /* cast before addition to avoid implicit widening */
    return poly_gcd_gf2(n_t, m_t);
}
