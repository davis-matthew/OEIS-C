/*
 * A348230  Partial sums of A348229.
 * https://oeis.org/A348230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=7; a(2)=23; for n≥3, let t(n)=8*n if n is odd, else
 *            t(n)=10*n-2 (n≠2). Then a(n)=a(n-1)+t(n). Equivalently: a(n)=23
 *            + 8·∑_{k odd,3≤k≤n}k + 10·∑_{k even,4≤k≤n}k – 2·#even(4≤k≤n)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)7;
    if (n == 2u) return (seq_t)23;
    /* n ≥ 3 */
    seq_t N = (seq_t)n;                 /* cast index to seq_t for arithmetic */
    seq_t M = N / (seq_t)2;             /* floor(n/2) */
    seq_t sum_all = (N * (N + (seq_t)1) / (seq_t)2) - (seq_t)3;  /* Σ_{k=3..n} k */
    seq_t sum_even = (M * (M + (seq_t)1) - (seq_t)2);            /* Σ_{even k=4..n} k */
    seq_t count_even = M - (seq_t)1;                                         /* #even in [4..n] */
    seq_t sum_odd = sum_all - sum_even;                                  /* Σ_{odd k=3..n} k */
    seq_t result = (seq_t)23
                   + (seq_t)8 * sum_odd
                   + (seq_t)10 * sum_even
                   - (seq_t)2 * count_even;
    return result;
}
