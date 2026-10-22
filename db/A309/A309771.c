/*
 * A309771  a(n) is the number of odd numbers found in the Collatz trajectory
 *          of 6*n + 3 before reaching a number x == 5 (mod 8).
 * https://oeis.org/A309771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5760, u32 n <= 9064, u64 n <= 9064,
 *            u128 n <= 9064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of odd terms in the Collatz trajectory of 6*n+3
 * until the first term congruent to 5 modulo 8 is reached. */
 
seq_t a(idx_t n)
{
    seq_t x = (seq_t)6 * (seq_t)n + (seq_t)3;   /* starting value 6*n+3 (odd) */
    seq_t cnt = 0;
    
    for (;;)
    {
        /* Count only odd numbers. */
        if ((x % 2) == 1)
        {
            ++cnt;
            if ((x % 8) == 5)   /* stop when congruent to 5 mod 8 */
                break;
        }
        
        /* Next Collatz step. */
        if ((x % 2) == 0)
            x = x / 2;          /* even case */
        else
            x = x * 3 + 1;      /* odd case 3x+1 */
    }
    
    return cnt;
}
