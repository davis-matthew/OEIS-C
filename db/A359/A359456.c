/*
 * A359456  Characteristic function of Fibonorial numbers.
 * https://oeis.org/A359456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 1 if n equals the product of the first k Fibonacci
 *            numbers for some k (F(1)*F(2)*…*F(k)), otherwise a(n) = 0
 * Range:     u8 n <= 5274, u16 n <= 9356, u32 n <= 9356, u64 n <= 9356,
 *            u128 n <= 9356, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; values for n < 1 are undefined, we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Current Fibonacci numbers: F(1)=1, F(2)=1, ... */
    seq_t f_prev = (seq_t)1;   /* F(k-1) */
    seq_t f_curr = (seq_t)1;   /* F(k)   */
    seq_t prod   = (seq_t)1;   /* Product of first k Fibonacci numbers */

    /* First product is 1 (F(1)). */
    if (prod == (seq_t)n)
        return (seq_t)1;

    /* Multiply by the second Fibonacci number (which is 1). */
    prod *= f_curr;
    if (prod == (seq_t)n)
        return (seq_t)1;

    /* Advance to the third Fibonacci number. */
    seq_t f_next = f_prev + f_curr;  /* F(3) = 1 + 1 = 2 */
    f_prev = f_curr;
    f_curr = f_next;

    /* Iterate multiplying by successive Fibonacci numbers until
       the product exceeds n or matches n. */
    for (;;)
    {
        prod *= f_curr;
        if (prod == (seq_t)n)
            return (seq_t)1;

        if (prod > (seq_t)n)
            return (seq_t)0;

        /* Compute next Fibonacci number. */
        f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
