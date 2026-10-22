/*
 * A345991  Smallest number of the form m*(m+1)*(m+2) that is divisible by n.
 * https://oeis.org/A345991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m*(m+1)*(m+2) such that n divides m*(m+1)*(m+2)
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value by a brute‑force search over m ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    for (idx_t m = 1; ; ++m) {
        /* Compute (m % n) * ((m+1) % n) % n safely using 128‑bit temp. */
        idx_t r = (unsigned __int128)(m % n) * ((m + 1) % n) % n;
        r = (unsigned __int128)r * ((m + 2) % n) % n;
        if (r == 0) {
            /* Product fits in seq_t for the tested range of n. */
            return (seq_t)m * (m + 1) * (m + 2);
        }
    }
}
