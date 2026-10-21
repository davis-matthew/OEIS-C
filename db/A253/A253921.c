/*
 * A253921  Indices of octagonal numbers (A000567) which are also centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A253921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 482*a(n-2) - 482*a(n-3) - a(n-4) + a(n-5)
 *            Initial values: a(1)=1, a(2)=51, a(3)=271, a(4)=24421,
 *            a(5)=130461
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: sequence starts at n = 1 */
    if (n == 0)
        return 0;          /* Undefined for n=0; return 0 as safe default */

    /* Initial terms of the sequence */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)51;
    seq_t a3 = (seq_t)271;
    seq_t a4 = (seq_t)24421;
    seq_t a5 = (seq_t)130461;

    /* Return early for the first five positions */
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;

    /* Compute subsequent terms iteratively using the recurrence.
       only the last five terms are kept, so memory usage is O(1). */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = a5
                    + (seq_t)482 * a4
                    - (seq_t)482 * a3
                    - a2
                    + a1;

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
