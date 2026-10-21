/*
 * A278313  Number of letters "I" in Roman numeral representation of n.
 * https://oeis.org/A278313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = [1,2,3,1,0][(n-1) mod 5] (period 5: a(n)=a(n-5) for n>5)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* under the offset – undefined but safe */
        return 0;

    /* (n-1) mod 5 */
    idx_t r = (n + (idx_t)-1) % 5;   /* idx_t is unsigned, wrap is safe after guard */

    /* Periodic table of values 1,2,3,1,0 */
    static const seq_t tbl[5] = {1, 2, 3, 1, 0};

    return tbl[r];
}
