/*
 * A250633  Number of (1+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n‑1) - 2*a(n‑2) - 14*a(n‑3) + 7*a(n‑4) + 10*a(n‑5) -
 *            5*a(n‑6) - 2*a(n‑7) + a(n‑8) Initial values: a(1)=36, a(2)=154,
 *            a(3)=585, a(4)=2183, a(5)=7924, a(6)=28456, a(7)=101308,
 *            a(8)=358990
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first eight terms of the sequence */
    static const seq_t init[8] = {
        (seq_t)36,
        (seq_t)154,
        (seq_t)585,
        (seq_t)2183,
        (seq_t)7924,
        (seq_t)28456,
        (seq_t)101308,
        (seq_t)358990
    };

    if (n <= 8U)               /* offset 1: a(1) is first term */
        return init[n - 1];

    /* keep the last eight terms: seq[0]..seq[7] are
       a(n‑8), a(n‑7), … , a(n‑1)  (before computing a(n)) */
    seq_t seq[8] = {
        init[0], init[1], init[2], init[3],
        init[4], init[5], init[6], init[7]
    };

    for (idx_t i = 9; i <= n; ++i) {
        /* compute 5*a(n‑1) + 7*a(n‑4) + 10*a(n‑5) + a(n‑8)  */
        seq_t pos = seq[7] * 5
                  + seq[4] * 7
                  + seq[3] * 10
                  + seq[0];

        /* compute 2*a(n‑2) + 14*a(n‑3) + 5*a(n‑6) + 2*a(n‑7) */
        seq_t neg = seq[6] * 2
                  + seq[5] * 14
                  + seq[2] * 5
                  + seq[1] * 2;

        seq_t next = pos - neg;               /* a(i)  */

        /* shift left: a(n‑7) becomes a(n‑8), … , a(n‑1) becomes a(n‑2) */
        seq[0] = seq[1];
        seq[1] = seq[2];
        seq[2] = seq[3];
        seq[3] = seq[4];
        seq[4] = seq[5];
        seq[5] = seq[6];
        seq[6] = seq[7];
        seq[7] = next;                       /* now a(n‑1) */
    }

    return seq[7];   /* a(n) stored in seq[7] after loop */
}
