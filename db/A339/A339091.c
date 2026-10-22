/*
 * A339091  a(1)=a(2)=1; thereafter if n is even, a(n) is the cumulative sum
 *          of the odd digits so far; and if n is odd, a(n) is the cumulative
 *          sum of the even digits so far.
 * https://oeis.org/A339091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339091
   a(1) = a(2) = 1;
   For n > 2:
      if n is even → a(n) is the cumulative sum of all odd digits
                      seen in a(1), a(2), …, a(n‑1);
      if n is odd  → a(n) is the cumulative sum of all even digits
                      seen in a(1), a(2), …, a(n‑1).
   This implementation iterates from 3 up to n, maintaining the two
   cumulative sums and updating them with the digits of each term. */

seq_t a(idx_t n)
{
    /* The first two terms are both 1. */
    if (n <= 2)
        return (seq_t)1;

    /* Cumulative sums of odd and even digits seen so far. */
    seq_t sumOdd  = 2;   /* two 1's contribute 2 to the odd sum */
    seq_t sumEven = 0;
    seq_t current = 0;   /* holds a(i) during the loop */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Choose the appropriate cumulative sum for the new term. */
        if ((i & 1ULL) == 0ULL)   /* i is even */
            current = sumOdd;
        else                      /* i is odd */
            current = sumEven;

        /* Update the cumulative sums with the digits of the new term. */
        seq_t x = current;
        /* If current is 0, its only digit is 0 (even), which adds nothing. */
        while (x > 0)
        {
            seq_t d = x % 10;
            if ((d & 1UL) == 0UL)
                sumEven += d;
            else
                sumOdd  += d;
            x /= 10;
        }
    }
    return current;  /* a(n) */
}
