/*
 * A315131  Coordination sequence Gal.5.186.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + f(n) where f(n) depends on n mod 6: if n < 6 :
 *            a(0)=1, a(1)=6, a(2)=10, a(3)=12, a(4)=18, a(5)=22 else if r==0
 *            : a = 4*n + 8*q else if r==1 or r==5 : a = 4*n + 4*q + 2 else if
 *            r==2 or r==4 : a = 4*n + 8*q + 2 else (r==3) : a = 4*n + 4 where
 *            q = floor(n/6), r = n mod 6
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small‑n exact values */
    static const seq_t init[6] = {1, 6, 10, 12, 18, 22};
    if (n < 6) return init[n];

    /* compute q = n/6 and r = n%6 */
    idx_t q_idx = n / 6;
    unsigned r = (unsigned)(n % 6);
    seq_t q = (seq_t)q_idx;
    seq_t n_seq = (seq_t)n;
    seq_t res;

    switch (r) {
        case 0:
            res = 4 * n_seq + 8 * q;
            break;
        case 1:
        case 5:
            res = 4 * n_seq + 4 * q + 2;
            break;
        case 2:
        case 4:
            res = 4 * n_seq + 8 * q + 2;
            break;
        case 3:
            res = 4 * n_seq + 4;
            break;
        default:  /* unreachable */
            res = 0;
            break;
    }
    return res;
}
