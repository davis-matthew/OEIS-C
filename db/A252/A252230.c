/*
 * A252230  Triangular array T read by rows: for j = k+1..2*k, k >=1, T(j,k) =
 *          least number of iterations of (h,i) -> (i,h-i) needed to take
 *          (k,j) to (k',j') satisfying k' <= j'.
 * https://oeis.org/A252230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A252230.  For a given index n (≥1), the
   corresponding row k and column j are determined by the triangular
   indexing, and the minimal h is found by iterating the Fibonacci
   ratio inequalities:
      odd h :  F(h+2)/F(h+1) ≤ j/k
      even h:  F(h+2)/F(h+1) ≥ j/k
   where F is the Fibonacci sequence with F(0)=0, F(1)=1. */
seq_t a(idx_t n)
{
    /* Find the row k such that k(k−1)/2 < n ≤ k(k+1)/2. */
    idx_t k = 1;
    while ((k * (k + 1) / 2) < n) {
        ++k;
    }

    /* Column j = k+1 + (n − s_k).  Here s_k = (k−1)k/2 + 1. */
    idx_t j = n + k - (k - 1) * k / 2;

    /* Minimal h by Fibonacci ratio test. */
    seq_t f_prev = (seq_t)1;   /* F(2)  */
    seq_t f_cur  = (seq_t)2;   /* F(3)  */
    seq_t k_s = (seq_t)k;
    seq_t j_s = (seq_t)j;

    for (seq_t h = 1;; ++h) {
        if (h & 1) {                /* h odd */
            if (f_cur * k_s <= j_s * f_prev) {
                return h;
            }
        } else {                     /* h even */
            if (f_cur * k_s >= j_s * f_prev) {
                return h;
            }
        }
        /* Update Fibonacci numbers for next h. */
        seq_t next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur = next;
    }
}
