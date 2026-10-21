/*
 * A250128  Number of triforces generated at iteration n in a Koch-Sierpiński
 *          Ninja Star.
 * https://oeis.org/A250128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 3*a(n-3); a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;

    seq_t a0 = 0, a1 = 1, a2 = 3;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 2 * a2 + 3 * a1 + 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
