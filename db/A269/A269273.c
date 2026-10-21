/*
 * A269273  Number of n X 5 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 234*a(n-1) - 14277*a(n-2) + 68796*a(n-3) - 86436*a(n-4)
 *            with initial values: a(1) = 432 a(2) = 127512 a(3) = 23549400
 *            a(4) = 3719884392 This recurrence holds for all n >= 5
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset 1 */
    if (n == 1) return 432;
    if (n == 2) return 127512;
    if (n == 3) return 23549400;
    if (n == 4) return 3719884392;

    /* Initialize the last four terms */
    seq_t a1 = 432;        /* a(n-4) at start corresponds to a(1) */
    seq_t a2 = 127512;    /* a(n-3) -> a(2) */
    seq_t a3 = 23549400;  /* a(n-2) -> a(3) */
    seq_t a4 = 3719884392;/* a(n-1) -> a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 234 * a4 - 14277 * a3 + 68796 * a2 - 86436 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
