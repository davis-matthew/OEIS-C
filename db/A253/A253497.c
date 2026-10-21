/*
 * A253497  Number of (3+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400*3^(n-3) + 415*2^(n-1) + 1626 (for n > 6) Initial
 *            values for n = 1..6: 1388, 2640, 4196, 6476, 11937, 25715
 * Range:     u8 none, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small‑n lookup table */
    static const seq_t init[7] = {0, 1388, 2640, 4196, 6476, 11937, 25715};

    if (n < 1)
        return 0;

    if (n <= 6)
        return init[n];

    /* compute 3^(n-3) */
    seq_t p3 = 1;
    for (idx_t i = 0; i < n - 3; ++i)
        p3 = p3 * 3;

    /* compute 2^(n-1) */
    seq_t p2 = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        p2 = p2 * 2;

    /* final combination */
    return (seq_t)400 * p3 + (seq_t)415 * p2 + 1626;
}
