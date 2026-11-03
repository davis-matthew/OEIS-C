/*
 * A256185  Second of two variations by Per Nørgård of his "infinity
 *          sequence", cf. A004718: v(0) = 0; v(3*n) = -v(n); v(3*n+1) = v(n)
 *          - 3; v(3*n+2) = -2 - v(n).
 * https://oeis.org/A256185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9478 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  v(0) = 0
 *               v(3*n)     = -v(n)
 *               v(3*n + 1) =  v(n) - 3
 *               v(3*n + 2) = -2 - v(n)
 *
 * This implements OEIS A256185.  The function works for any idx_t
 * (uint64_t) and any seq_t integer type, including arbitrary‑precision
 * big integers.  The recursion depth is O(log₃ n), which is at most
 * 20 for 64‑bit indices, so it easily meets the time constraints.      */
static seq_t calc_prime_noergard(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* base case */
    idx_t q = n / 3;
    seq_t base = calc_prime_noergard(q);   /* recursion on the quotient */

    switch (n % 3) {
    case 0:
        return -base;                /* v(3*n)     = -v(n)           */
    case 1:
        return base - (seq_t)3;      /* v(3*n + 1) =  v(n) - 3       */
    default: /* case 2 */
        return -(seq_t)2 - base;     /* v(3*n + 2) = -2 - v(n)       */
    }
}

seq_t a(idx_t n)
{
    return calc_prime_noergard(n);
}
