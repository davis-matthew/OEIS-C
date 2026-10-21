/*
 * A255070  (1/2)*(n minus number of runs in the binary expansion of n): a(n)
 *          = (n - A005811(n)) / 2 = A236840(n)/2.
 * https://oeis.org/A255070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - A005811(n)) / 2 where A005811(n) = number of runs in
 *            the binary expansion of n (runs = 1 + number of adjacent unequal
 *            bits)
 * Range:     u8 n <= 514, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    idx_t temp = n;
    unsigned prev_bit = temp & 1u;    /* least‑significant bit */
    seq_t runs = 1;                   /* count the first run */

    temp >>= 1;
    while (temp != 0u) {
        unsigned cur_bit = temp & 1u;
        if (cur_bit != prev_bit)
            ++runs;
        prev_bit = cur_bit;
        temp >>= 1;
    }

    seq_t n_s = (seq_t)n;
    return (n_s - runs) / 2;
}
