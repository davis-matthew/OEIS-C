/*
 * A339012  Written in factorial base, n ends in a(n) consecutive non-0
 *          digits.
 * https://oeis.org/A339012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1744, u16 n <= 9980, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of trailing non‑zero digits in the factorial
   (factoradic) representation of n, where the least‑significant digit is
   position 0 (1! digit).  The algorithm repeatedly extracts the
   factoradic digits by successive division by (k+1) and stops when the
   first zero digit is reached. */
seq_t a(idx_t n)
{
    seq_t r = (seq_t)n;          /* working copy of n in seq_t */
    seq_t count = 0;
    for (idx_t k = 1; r; ++k) {  /* while there are still higher digits */
        seq_t digit = r % (k + 1);   /* current factoradic digit (0 <= digit <= k) */
        if (digit == 0)              /* zero digit marks the end of trailing non‑zero digits */
            break;
        ++count;                     /* count a non‑zero trailing digit */
        r /= (k + 1);                /* remove this digit and continue with higher digits */
    }
    return count;
}
