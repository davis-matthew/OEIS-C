/*
 * A391614  a(4n) = a(2n+a(n)), a(4n+2) = a(2n+1-a(n)) and a(2n+1) = 1-a(2n),
 *          with a(0) = 0.
 * https://oeis.org/A391614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391614
   a(0) = 0;
   a(2n+1) = 1 - a(2n);
   a(4n)   = a(2n + a(n));
   a(4n+2) = a(2n + 1 - a(n));
   All values are 0 or 1. This implementation evaluates the recurrence
   recursively, guaranteeing termination because each recursive call
   arguments are strictly smaller. The depth of recursion is O(log n). */
seq_t a(idx_t n)
{
    if (n == 0)          /* base case */
        return (seq_t)0;

    if ((n & 1) != 0) {  /* odd n : n = 2k + 1 */
        return ((seq_t)1) - a(n - 1);
    } else {              /* even n */
        idx_t k = n >> 2; /* n / 4   (k is non‑negative) */
        if ((n & 3) == 0)      /* n % 4 == 0 : n = 4k */
        {
            /* term = 2k + a(k)  (all sub‑exponents are <= n/2) */
            idx_t term = (idx_t)(2 * k + (idx_t)a(k));
            return a(term);
        } else                  /* n % 4 == 2 : n = 4k + 2 */
        {
            /* term = 2k + 1 - a(k) */
            idx_t term = (idx_t)(2 * k + 1 - (idx_t)a(k));
            return a(term);
        }
    }
}
