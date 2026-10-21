/*
 * A254068  Irregular triangle T read by rows in which the entry in row n and
 *          column k is given by T(n,k) = 4*A253676(n,k) - 3, k =
 *          1..A253720(n), assuming the 3x+1 (or Collatz) conjecture.
 * https://oeis.org/A254068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate rows of the Collatz reduced sequence starting from 4*n‑3;
   collect every odd odd number congruent to 1 modulo 4 until 1 is reached.
   Count these numbers; when the counter reaches the requested index n,
   return the corresponding value.  This implements the definition of A254068. */

seq_t a(idx_t n)
{
    /* The sequence is 1‑based.  For n < 1 the behaviour is undefined. */
    if (n < 1) return (seq_t)0;

    idx_t count = 0;          /* how many qualifying numbers have been seen */

    for (idx_t r = 1; ; ++r)   /* generate rows 1,2,3,… */
    {
        seq_t x = (seq_t)(4 * r - 3);   /* starting value 4*r‑3 */

        for (;;)
        {
            /* If x ≡ 1 (mod 4), it enters the triangle. */
            if ((x & 3) == 1)          /* (x % 4 == 1) but bitwise is faster */
            {
                count++;
                if (count == n)        /* nth element found */
                    return x;
            }

            if (x == 1)                /* finished this row */
                break;

            /* Next odd element in the Collatz reduced chain:
               compute 3*x+1, then strip out all factors of 2.   */
            seq_t t = 3 * x + 1;       /* 3*x + 1 */
            while ((t & 1) == 0)       /* while t is even, divide by 2 */
                t >>= 1;
            x = t;
        }
    }
}
