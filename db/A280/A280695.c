/*
 * A280695  a(n) = n / A280694(n); n divided by the largest Lucas number
 *          (A000032) dividing n.
 * https://oeis.org/A280695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A280694(n)
 * Range:     u8 n <= 255, u16 n <= 9874, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A280694(n) is the largest Lucas number dividing n.  */
/* The function searches the Lucas numbers in ascending order
   using the recurrence L(k+1)=L(k)+L(k-1) and keeps the biggest
   divisor found.  */

seq_t a(idx_t n)
{
    /* For n = 0 the function returns 0, though the offset begins at 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t sn   = (seq_t)n;   /* cast the index to the sequence type       */
    seq_t best = (seq_t)1;    /* L(1) = 1 always divides any n              */
    /* Check Lucas(0) = 2 */
    seq_t L0 = (seq_t)2;
    if (sn % L0 == 0)
        best = L0;

    /* Iterate Lucas numbers starting with L(2) = 3 */
    seq_t prev = L0;                    /* L(k-1)  */
    seq_t curr = (seq_t)1;              /* L(k)    */
    while (1) {
        seq_t next = prev + curr;       /* L(k+1)  */
        if (next > sn)                  /* Stop when exceeding n     */
            break;
        if (sn % next == 0)
            best = next;                /* Keep the largest divisor  */
        prev = curr;
        curr = next;
    }
    return sn / best;                    /* n divided by the largest Lucas divisor */
}
