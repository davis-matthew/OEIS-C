/*
 * A387762  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*k,2*n-4*k).
 * https://oeis.org/A387762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387762
 * Formula: a(n) = Σ_{k=0..⌊n/2⌋} 2^k * binomial(2k, 2n-4k)
 * Recurrence: a(n) = 4*a(n-2)+4*a(n-3)-4*a(n-4)+8*a(n-5)-4*a(n-6)   (n≥6)
 * Initial values:
 *   a(0)=1, a(1)=0, a(2)=2, a(3)=2, a(4)=4, a(5)=24
 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 2;
    if (n == 4) return 4;
    if (n == 5) return 24;

    /* keep the last six computed values */
    seq_t v[6] = {1, 0, 2, 2, 4, 24};

    for (idx_t i = 6; i <= n; ++i) {
        /* next = 4*v[4] + 4*v[3] + 8*v[1] - 4*(v[2] + v[0]) */
        seq_t temp = 4 * v[4] + 4 * v[3] + 8 * v[1];
        seq_t sub  = 4 * (v[2] + v[0]);
        seq_t next = temp - sub;

        /* shift the window */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];
}
