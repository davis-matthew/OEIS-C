/*
 * A298022  Coordination sequence for Dual(3^3.4^2) tiling with respect to a
 *          trivalent node.
 * https://oeis.org/A298022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) – a(n-4) for n ≥ 9
   Initial values (OEIS A298022):
   a(0)=1, a(1)=3, a(2)=7, a(3)=12, a(4)=17,
   a(5)=23, a(6)=28, a(7)=33, a(8)=37 */
seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        1, 3, 7, 12, 17, 23, 28, 33, 37
    };

    if (n < 9)
        return init[n];

    /* Current values for a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a0 = init[5]; /* a(5)  */
    seq_t a1 = init[6]; /* a(6)  */
    seq_t a2 = init[7]; /* a(7)  */
    seq_t a3 = init[8]; /* a(8)  */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val = a3 + a1 - a0;  /* a(i) = a(i‑1) + a(i‑3) – a(i‑4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;
}
