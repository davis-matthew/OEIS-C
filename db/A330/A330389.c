/*
 * A330389  Maximal order of an element in the n X n X n Rubik's cube group.
 * https://oeis.org/A330389
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 45, a(3) = 1260, a(4) = 765765, a(5) = 281801520, a(n) =
 *            5354228880 for all n ≥ 6. (a(1), a(0) undefined)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2U) return (seq_t)45U;
    if (n == 3U) return (seq_t)1260U;
    if (n == 4U) return (seq_t)765765U;
    if (n == 5U) return (seq_t)281801520U;
    if (n >= 6U) return (seq_t)5354228880U;
    /* For n < 2 the sequence is not defined; return 0. */
    return (seq_t)0U;
}
