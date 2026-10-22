/*
 * A365907  Smallest nonnegative integer that is not the sum of fewer than n
 *          signed Lucas numbers.
 * https://oeis.org/A365907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, for n < 2. a(n) = (A000032(3*n-2)+3)/2, for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the k-th Lucas number L(k), where L(0)=2, L(1)=1. */
static seq_t lucas(idx_t k)
{
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;

    seq_t prev = (seq_t)2; /* L(0) */
    seq_t curr = (seq_t)1; /* L(1) */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t next = prev + curr; /* L(i) = L(i-1) + L(i-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)n;

    /* Index in Lucas sequence: k = 3*n-2. */
    idx_t k = 3 * n - 2;

    seq_t lucas_val = lucas(k);
    return (lucas_val + (seq_t)3) / (seq_t)2;
}
