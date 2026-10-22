/*
 * A372986  a(n) = (2*n)!/a(n-1), with a(0)=1.
 * https://oeis.org/A372986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Product_{k=0..floor((n-1)/2)} 2*(n-2*k)*(2*n-4*k-1)
 * Recurrence: a(0)=1, a(n) = (2*n)! / a(n-1)
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;
    idx_t max_k = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = (seq_t)2
                     * ((seq_t)n - (seq_t)2 * k)
                     * ((seq_t)2 * n - (seq_t)4 * k - (seq_t)1);
        res *= term;
    }

    return res;
}
