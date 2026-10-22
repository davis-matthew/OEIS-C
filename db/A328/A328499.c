/*
 * A328499  The number of primitive Pythagorean triangles with perimeter less
 *          than n.
 * https://oeis.org/A328499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primitive Pythagorean triples with perimeter ≤
 *            n (OEIS A328499, offset 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euclidean GCD for uint64_t, used in the coprimality test. */
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = b; b = a % b; a = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* avoid defining a(0), though unused */

    seq_t count = (seq_t)0;

    /* m starts at 2.  The smallest perimeter for given m is
       P_min = 2*m*(m+1).  We need P_min ≤ n to possibly have a triple. */
    for (idx_t m = 2; 2 * m * (m + 1) <= n; ++m) {
        for (idx_t k = 1; k < m; ++k) {
            /* Primitive condition: m and k coprime and of opposite parity. */
            if (((m - k) & 1) && _gcd_u64((uint64_t)m, (uint64_t)k) == 1) {
                seq_t P = (seq_t)2 * m * (m + k);
                if (P <= n) {
                    ++count;
                }
            }
        }
    }
    return count;
}
