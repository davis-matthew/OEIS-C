/*
 * A348536  Number of partitions of n into 3 parts that divide n.
 * https://oeis.org/A348536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A348536 – number of partitions of n into 3 parts that divide n.
 *  The sequence is periodic with period 12, as proven by the solutions
 *  to 1/m1 + 1/m2 + 1/m3 = 1.  The values for one full period are:
 *      n mod 12 : a(n)
 *          1 -> 0
 *          2 -> 0
 *          3 -> 1
 *          4 -> 1
 *          5 -> 0
 *          6 -> 2
 *          7 -> 0
 *          8 -> 1
 *          9 -> 1
 *         10 -> 0
 *         11 -> 0
 *         12 -> 3
 *
 *  The function returns a(n) for any n >= 1 using this periodicity.
 */
static const seq_t seq_a[12] = {
    0, 0, 1, 1, 0, 2, 0, 1, 1, 0, 0, 3
};

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 to avoid UB. */
    if (n == 0)
        return 0;
    /* The sequence repeats every 12 terms. */
    idx_t r = (n - 1) % 12;   /* r ∈ {0,…,11} */
    return seq_a[r];
}
