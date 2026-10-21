/*
 * A251645  Number of (n+2) X (1+2) 0..3 arrays with no row, column, diagonal
 *          or antidiagonal in any 3 X 3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 66, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (valid for n > 14):
   a(n) = a(n-1) + 2*a(n-2) + 5*a(n-3) - 7*a(n-4) - 12*a(n-5) + 6*a(n-6) + 6*a(n-7)
   Initial values:
   a(1)=376,  a(2)=448,  a(3)=738,  a(4)=1248,  a(5)=2382,  a(6)=4140,
   a(7)=6978,  a(8)=13532,  a(9)=23574, a(10)=39904, a(11)=78046, a(12)=136408,
   a(13)=231290, a(14)=455168 */
seq_t a(idx_t n)
{
    /* Base values for n = 1 .. 14 */
    static const seq_t base[14] = {
        (seq_t)376,  (seq_t)448,  (seq_t)738,   (seq_t)1248,  (seq_t)2382,
        (seq_t)4140, (seq_t)6978, (seq_t)13532, (seq_t)23574, (seq_t)39904,
        (seq_t)78046, (seq_t)136408, (seq_t)231290, (seq_t)455168
    };

    if (n <= 14)
        return base[n - 1];

    /* Ring buffer holding the last 7 terms:
       prev[0] = a(i-1), prev[1] = a(i-2), ..., prev[6] = a(i-7) */
    seq_t prev[7];
    prev[0] = base[13]; /* a(14) */
    prev[1] = base[12]; /* a(13) */
    prev[2] = base[11]; /* a(12) */
    prev[3] = base[10]; /* a(11) */
    prev[4] = base[9];  /* a(10) */
    prev[5] = base[8];  /* a(9)  */
    prev[6] = base[7];  /* a(8)  */

    for (idx_t i = 15; i <= n; ++i) {
        seq_t pos = prev[0] + 2 * prev[1] + 5 * prev[2]
                     + 6 * prev[5] + 6 * prev[6];
        seq_t neg = 7 * prev[3] + 12 * prev[4];
        seq_t cur = pos - neg;          /* a(i) */

        /* Shift ring buffer for next iteration */
        for (int j = 6; j > 0; --j)
            prev[j] = prev[j - 1];
        prev[0] = cur;
    }

    return prev[0];   /* a(n) */
}
