/*
 * A253887  Row index of n in A191450: a(3n) = 2n, a(3n+1) = 2n+1, a(3n+2) =
 *          a(n+1).
 * https://oeis.org/A253887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253887: row index of n in A191450
   Recurrence:  a(3n)   = 2n
                a(3n+1) = 2n+1
                a(3n+2) = a(n+1)
   Initial values: a(1)=1  (a(2)=1, a(3)=2, …) */

/* No headers are required except the one defining seq_t and idx_t. */

static seq_t _a_rec(idx_t n)
{
    /* The offset is 1, so n ≥ 1.  The recurrence works directly. */
    idx_t r = n % 3;
    if (r == 0)           /* n = 3k  */
        return (seq_t)(n / 3) * 2;
    else if (r == 1)      /* n = 3k+1 */
        return (seq_t)((n - 1) / 3) * 2 + 1;
    else                  /* n = 3k+2 */
        return _a_rec((n + 1) / 3);
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
