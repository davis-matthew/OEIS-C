/*
 * A296913  Number of ways to properly color the Petersen graph using n
 *          colors.
 * https://oeis.org/A296913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 - 15*n^9 + 105*n^8 - 455*n^7 + 1353*n^6 * - 2861*n^5
 *            + 4275*n^4 - 4305*n^3 + 2606*n^2 - 704*n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 85, u128 n <= 7133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Ensure n is treated as seq_t for all arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t pow_n  = 1;          /* will hold n^k */

    seq_t pos_sum = 0;         /* sum of positive coefficient terms   */
    seq_t neg_sum = 0;         /* sum of negative coefficient terms   */

    /* Compute powers up to n^10 and accumulate positive / negative terms */
    for (int k = 1; k <= 10; ++k) {
        pow_n = pow_n * n_seq;   /* now pow_n = n^k */

        switch (k) {
            case 1:   neg_sum += (seq_t)704 * pow_n;   break; /* -704*n^1 */
            case 2:   pos_sum += (seq_t)2606 * pow_n;  break; /* +2606*n^2 */
            case 3:   neg_sum += (seq_t)4305 * pow_n;  break; /* -4305*n^3 */
            case 4:   pos_sum += (seq_t)4275 * pow_n;  break; /* +4275*n^4 */
            case 5:   neg_sum += (seq_t)2861 * pow_n;  break; /* -2861*n^5 */
            case 6:   pos_sum += (seq_t)1353 * pow_n;  break; /* +1353*n^6 */
            case 7:   neg_sum += (seq_t)455  * pow_n;  break; /* -455*n^7 */
            case 8:   pos_sum += (seq_t)105  * pow_n;  break; /* +105*n^8 */
            case 9:   neg_sum += (seq_t)15   * pow_n;  break; /* -15*n^9 */
            case 10:  pos_sum +=            pow_n;     break; /* +1*n^10 */
        }
    }

    /* The polynomial is non‑negative for n ≥ 1, so pos_sum ≥ neg_sum */
    return pos_sum - neg_sum;
}
