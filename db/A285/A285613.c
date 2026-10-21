/*
 * A285613  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 62", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10; for n>=2 a(n) = ((10^⌊n/2⌋ - 1)/9) *
 *            10^⌊(n+1)/2⌋ This is equivalent to a decimal number consisting
 *            of ⌊n/2⌋ ones followed by ⌊(n+1)/2⌋ zeros
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two exceptional cases. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    /* For n >= 2 */
    idx_t ones  = n / 2;        /* floor(n/2) */
    idx_t zeros = (n + 1) / 2;  /* floor((n+1)/2) */

    seq_t val = 0;

    /* Append the 'ones' number of '1' digits. */
    for (idx_t i = 0; i < ones; ++i)
        val = val * (seq_t)10 + (seq_t)1;

    /* Append the 'zeros' number of '0' digits. */
    for (idx_t i = 0; i < zeros; ++i)
        val = val * (seq_t)10;

    return val;
}
