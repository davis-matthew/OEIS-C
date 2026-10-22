/*
 * A352717  Greatest Lucas number that does not exceed n.
 * https://oeis.org/A352717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: returns the greatest Lucas number not exceeding n.
   The Lucas numbers are defined as L_0 = 1, L_1 = 3, L_k = L_{k-1} + L_{k-2}
   (i.e., 1, 3, 4, 7, 11, 18, …).
   The function iterates the sequence until a term exceeds n and
   returns the previous term. */
seq_t a(idx_t n) {
    seq_t res   = 1;           /* value to return */
    seq_t prev  = 1;           /* L_{k-1}   */
    seq_t curr  = 3;           /* L_k       */
    seq_t limit = (seq_t)n;    /* upper bound */

    /* for n=1 the loop never runs and res stays 1 */
    while (curr <= limit) {
        res   = curr;          /* current term is ≤ n */
        seq_t next = prev + curr;
        /* detect unsigned overflow: if the sum wrapped around
           it will be ≤ curr, so stop here */
        if (next <= curr) break;
        prev = curr;
        curr = next;
    }
    return res;
}
