/*
 * A244762  a(n) = (5*3^n-2*n-1)/4.
 * https://oeis.org/A244762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n+1) = 3*a(n) + n */
/* This implements OEIS A244762: a(n) = (5*3^n-2*n-1)/4 */
/* It works for any seq_t/idx_t provided by the build. */
seq_t a(idx_t n)
{
    /* Start with a(0) = 1. */
    seq_t value = 1;
    if (n == 0)
        return value;

    /* Compute using the recurrence a(n+1) = 3*a(n) + n. */
    for (idx_t i = 0; i < n; ++i) {
        /* value holds a(i); compute a(i+1). */
        value = 3 * value + (seq_t)i;
    }
    return value;
}
