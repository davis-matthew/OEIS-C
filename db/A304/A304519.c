/*
 * A304519  a(n) = 72*2^n - 56 (n>=1).
 * https://oeis.org/A304519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 72*2^n - 56
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=88, a(2)=232, a(n)=3*a(n-1)-2*a(n-2) for n>2 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1. Behaviour for n < 1 is irrelevant. */
    if (n == 0) return 0;          /* arbitrary, not used by the tests */
    if (n == 1) return 88;
    if (n == 2) return 232;

    seq_t prev2 = 88;              /* a(n-2) */
    seq_t prev1 = 232;             /* a(n-1) */
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t cur = 3 * prev1 - 2 * prev2;   /* a(n) = 3*a(n-1)-2*a(n-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
