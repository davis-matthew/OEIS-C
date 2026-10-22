/*
 * A396948  Numbers m with two or more digits such that if we insert a 0
 *          between the units and tens digits we get a multiple of the
 *          original number.
 * https://oeis.org/A396948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*(n-3) for n >= 8; the first seven terms are * listed
 *            explicitly: a(1)=10, a(2)=15, a(3)=18, a(4)=20, a(5)=30, *
 *            a(6)=40, a(7)=45
 * Range:     u8 n <= 28, u16 n <= 6556, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 10;
    if (n == 2) return 15;
    if (n == 3) return 18;
    if (n == 4) return 20;
    if (n == 5) return 30;
    if (n == 6) return 40;
    if (n == 7) return 45;
    /* For n >= 8 all numbers are multiples of 10 and the three
     * special terms 15, 18, 45 have already appeared. */
    return ((seq_t)(n - 3)) * 10;
}
