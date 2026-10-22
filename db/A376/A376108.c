/*
 * A376108  Non-Leonardo numbers.
 * https://oeis.org/A376108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer that is NOT of the form 2*F_k −
 *            1, where F_k is the k‑th Fibonacci number
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the Non‑Leonardo numbers. */
seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t x = 1;                 /* candidate integer */

    /* Fibonacci initialization: F1 = 1, F2 = 1. */
    seq_t f_prev = 1;            /* F(k-2)  */
    seq_t f_curr = 1;            /* F(k-1)  */
    seq_t next_excl = 1;         /* 2*F2 − 1 , the first excluded number */

    while (1) {
        /* Skip the excluded numbers. */
        if (x == next_excl) {
            /* Advance to the next Fibonacci number. */
            seq_t f_next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_next;

            /* New excluded number: 2*F_k − 1. */
            next_excl = f_curr * 2 - 1;
            x++;                 /* move to the next candidate */
            continue;
        }

        /* x is not excluded. */
        cnt++;
        if (cnt == n)
            return x;
        x++;
    }
}
