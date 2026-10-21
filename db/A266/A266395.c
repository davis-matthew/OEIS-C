/*
 * A266395  Number of orbits of Aut(Z^7) as function of the infinity norm n of
 *          the representative lattice point of the orbit, when the
 *          cardinality of the orbit is equal to 161280.
 * https://oeis.org/A266395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * (n-1) * (n-2) * (n-3) * (n-4) / 8
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 285, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        return (seq_t)0;
    }

    seq_t val = (seq_t)(n - 1);
    val *= (seq_t)(n - 2);
    val *= (seq_t)(n - 3);
    val *= (seq_t)(n - 4);
    val /= (seq_t)8;
    val *= (seq_t)5;

    return val;
}
