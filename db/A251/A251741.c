/*
 * A251741  8-step Fibonacci sequence starting with 0,0,0,0,1,0,0,0.
 * https://oeis.org/A251741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+8) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7);
 *            initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0,
 *            a(6)=0, a(7)=0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {0, 0, 0, 0, 1, 0, 0, 0};

    if (n < 8) {                     /* return a pre‑computed term */
        return init[n];
    }

    seq_t ring[8];                   /* holds the last 8 values */
    for (int i = 0; i < 8; ++i) ring[i] = init[i];

    seq_t sum = 0;                   /* sum of the current ring */
    for (int i = 0; i < 8; ++i) sum += ring[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = sum;                    /* a(i) */
        seq_t old  = ring[(int)(i % 8)];     /* a(i-8) */
        ring[(int)(i % 8)] = next;           /* store new value */
        sum = sum + next - old;              /* update running sum */
    }

    return ring[(int)(n % 8)];
}
