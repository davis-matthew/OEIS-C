/*
 * A291741  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = (1 - S)(1 + S^2).
 * https://oeis.org/A291741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=4, a(6)=5, a(7)=7,
 *            a(8)=11, a(9)=12; For n≥10:
 *            a(n)=a(n‑1)−a(n‑2)+2·a(n‑3)−2·a(n‑4)+3·a(n‑5)−a(n‑6)+3·a(n‑7)+a(n‑9)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 9 */
    static const seq_t init_vals[10] = {
        (seq_t)1,  (seq_t)0,  (seq_t)1,  (seq_t)1,  (seq_t)1,
        (seq_t)4,  (seq_t)5,  (seq_t)7,  (seq_t)11, (seq_t)12
    };

    /* If n is in the base range, return it directly */
    if (n < 10)
        return init_vals[n];

    /* Use a circular buffer to keep the last ten values.
       The buffer indexes correspond to indices modulo 10. */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init_vals[i];

    /* Compute values from 10 up to n */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t next =
            buf[(i - 1) % 10]           /* a(i-1)  */
          - buf[(i - 2) % 10]           /* a(i-2)  */
          + 2 * buf[(i - 3) % 10]       /* 2·a(i-3)*/
          - 2 * buf[(i - 4) % 10]       /* -2·a(i-4)*/
          + 3 * buf[(i - 5) % 10]       /* 3·a(i-5)*/
          - buf[(i - 6) % 10]           /* -a(i-6)  */
          + 3 * buf[(i - 7) % 10]       /* 3·a(i-7)*/
          + buf[(i - 9) % 10];          /* +a(i-9)  */

        buf[i % 10] = next;
    }

    return buf[n % 10];
}
