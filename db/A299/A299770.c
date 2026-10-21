/*
 * A299770  a(n) is the total number of elements after n-th stage of a hybrid
 *          (and finite) cellular automaton on the infinite square grid,
 *          formed by toothpicks of length 2, D-toothpicks, toothpicks of
 *          length 1, and T-toothpicks.
 * https://oeis.org/A299770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(10) for n ≥ 10. Initial terms are: a(1)=1, a(2)=5,
 *            a(3)=13, a(4)=21, a(5)=33, a(6)=49, a(7)=65, a(8)=73, a(9)=97,
 *            a(10)=105
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for the first 10 stages. */
    static const seq_t seq[10] = {
        1, 5, 13, 21, 33,
        49, 65, 73, 97, 105
    };

    /* Bounds: n is guaranteed to be ≥ 1. */
    if (n <= 10)
        return seq[n - 1u];

    /* After stage 10 the structure does not grow. */
    return seq[9];
}
