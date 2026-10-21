/*
 * A256654  Least Fibonacci number not less than n.
 * https://oeis.org/A256654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest Fibonacci number ≥ n * (with F(1)=1, F(2)=1,
 *            F(k)=F(k-1)+F(k-2) for k≥3)
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=1 (and n=0 which never occurs for the sequence offset) return 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t target = (seq_t)n;
    seq_t f_prev = 1;
    seq_t f_curr = 1;

    /* Increment Fibonacci numbers until f_curr ≥ target */
    while (f_curr < target) {
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }
    return f_curr;
}
