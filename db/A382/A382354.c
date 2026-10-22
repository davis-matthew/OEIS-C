/*
 * A382354  Triangle T(n,k) read by rows, where row n is a permutation of the
 *          numbers 1 through n, such that if a deck of n cards is prepared in
 *          this order, and under-down-under dealing is used, then the
 *          resulting cards will be dealt in increasing order.
 * https://oeis.org/A382354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle T(n,k) read by rows.
 *            T(n,k) is defined by the recurrence: T(n,2) = 1 T(1,1) = 1,
 *            T(2,1) = 2 T(n,1) = T(n-1,n-2) + 1 (n>2) T(n,3) = T(n-1,n-1) + 1
 *            (n>2) T(n,k) = T(n-1,k-3) + 1 (n>3, k>3) a(n) is obtained by
 *            flattening the triangle row‑wise (row1:1; row2:2,1;...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset 1 – not used */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        r += 1;
        sum += r;
    }
    /* sum now equals r*(r+1)/2; previous sum = sum - r */
    idx_t k = n - (sum - r);   /* 1 <= k <= r */

    /* Compute T(r,k) using the recurrence, counting the +1 at each step. */
    idx_t steps = 0;
    idx_t curr_n = r;
    idx_t curr_k = k;

    while (1) {
        if (curr_k == 2) {                     /* T(any,2) = 1 */
            return (seq_t)(1 + steps);
        }
        if (curr_n == 1 && curr_k == 1) {       /* base T(1,1)=1 */
            return (seq_t)(1 + steps);
        }
        if (curr_n == 2 && curr_k == 1) {       /* base T(2,1)=2 */
            return (seq_t)(2 + steps);
        }
        if (curr_k == 1) {                     /* T(n,1) = T(n-1,n-2)+1 */
            idx_t old_n = curr_n;
            curr_n -= 1;
            curr_k = old_n - 2;
            steps += 1;
            continue;
        }
        if (curr_k == 3) {                     /* T(n,3) = T(n-1,n-1)+1 */
            idx_t old_n = curr_n;
            curr_n -= 1;
            curr_k = old_n - 1;
            steps += 1;
            continue;
        }
        /* k > 3 : T(n,k) = T(n-1,k-3)+1 */
        curr_n -= 1;
        curr_k -= 3;
        steps += 1;
    }
}
