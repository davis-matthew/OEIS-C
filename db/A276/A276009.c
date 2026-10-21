/*
 * A276009  Decrement each nonzero digit by one in factorial base
 *          representation of n: a(n) = n - A276008(n).
 * https://oeis.org/A276009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 406, u16 n <= 8648, u32 n <= 8648, u64 n <= 8648,
 *            u128 n <= 8648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the factorial‐base number obtained by decrementing each
   nonzero digit of n’s factorial‐base representation. */
seq_t a(idx_t n)
{
    /* n = 0 is a special case; its factorial base has no digits. */
    if (n == 0)
        return 0;

    /* Find the highest factorial ≤ n.  k is the position index
       (1 for 1!, 2 for 2!, …).  fact = k! after the loop. */
    idx_t k = 0;
    seq_t fact = 1;                /* fact holds i! for the current i */
    while (1) {
        seq_t next = fact * (seq_t)(k + 1);      /* (k+1)! = k! * (k+1) */
        if (next > (seq_t)n)
            break;
        fact = next;
        k++;
    }

    seq_t remaining = (seq_t)n;   /* part of n still to be processed */
    seq_t result    = 0;           /* value after decrementing digits */

    /* Process factorial positions from k down to 1. */
    for (idx_t i = k; i > 0; ) {
        /* Current digit in position i (i! weight). */
        seq_t digit = remaining / fact;

        if (digit != 0) {
            seq_t new_digit = digit - 1;
            result += new_digit * fact;      /* accumulate the new number */
        }

        /* Remove the contribution of this digit from remaining. */
        remaining -= digit * fact;

        /* Prepare for the next lower factorial: (i-1)! = i! / i. */
        fact = fact / (seq_t)i;

        /* Break after handling the lowest position (i==1). */
        if (i == 1)
            break;
        i--;                                /* move to next lower i */
    }

    return result;
}
