/*
 * A263687  b(n) in (sqrt(2))_n = b(n) + c(n)*sqrt(2), where (x)_n is the
 *          Pochhammer symbol, b(n) and c(n) are integers.
 * https://oeis.org/A263687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from (sqrt(2))_n = (n-1+sqrt(2))*(sqrt(2))_(n-1)
   with (sqrt(2))_0 = 1 = 1 + 0*sqrt(2).
   Let a(n) + b(n)*sqrt(2) = (sqrt(2))_n.
   Then
        a(0) = 1,  b(0) = 0
        a(n) = a(n-1)*(n-1) + 2*b(n-1)
        b(n) = a(n-1) + b(n-1)*(n-1)    (n >= 1)
   The required sequence is a(n).  */

/* The function computes a(n) for n >= 0.  */
seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)1; /* a(0) */
    seq_t b_val = (seq_t)0; /* b(0) */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t ii   = (seq_t)(i - (idx_t)1);   /* n-1 as seq_t */
        seq_t new_a = a_val * ii + (seq_t)2 * b_val;
        seq_t new_b = a_val + b_val * ii;
        a_val = new_a;
        b_val = new_b;
    }

    return a_val;
}
