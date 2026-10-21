/*
 * A280511  Index sequence of the block-fractal sequence A001468.
 * https://oeis.org/A280511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest odd‑index Fibonacci number F(2k+1) such that
 *            n <= sum_{i=1..k} F(2i+1). Equivalently, a(n) is the odd‑indexed
 *            Fibonacci number whose cumulative sum of those Fibonacci numbers
 *            reaches the index n
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 for the sequence */

    seq_t sum  = 0;                      /* cumulative sum of odd Fibonacci
                                            numbers seen so far */
    seq_t prev = 1;                      /* F(1) */
    seq_t cur  = 1;                      /* F(2) */
    idx_t idx  = 2;                      /* current Fibonacci index */

    for (;;)
    {
        /* advance to the next Fibonacci number */
        idx++;
        seq_t next = prev + cur;         /* F(idx+1) */
        prev = cur;
        cur = next;                     /* now cur == F(idx) */

        if ((idx & 1U) && idx >= 3)     /* odd indices ≥ 3 start the blocks */
        {
            sum += cur;                 /* add this odd Fibonacci number */
            if (n <= sum)               /* threshold reached */
                return cur;             /* cur is the desired a(n) */
        }
    }
}
