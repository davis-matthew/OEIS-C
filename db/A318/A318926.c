/*
 * A318926  Take the binary expansion of n, starting with the least
 *          significant bit, and concatenate the lengths of the runs.
 * https://oeis.org/A318926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the lengths of runs of equal bits in the
 *            binary expansion of n, read from the least significant bit to
 *            the most significant
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 1364, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;  /* result */
    if (n == 0) return 0;   /* offset starts at 1, but safe for n=0 */
    idx_t t = n;
    int curr_bit = t & 1;
    int run_len  = 0;

    while (t > 0) {
        int bit = t & 1;
        if (bit == curr_bit) {
            ++run_len;
        } else {
            /* append the previous run length */
            seq_t mult = (run_len < 10) ? (seq_t)10 : (seq_t)100;
            ans = ans * mult + run_len;
            curr_bit = bit;
            run_len  = 1;
        }
        t >>= 1;
    }

    /* append the last run length */
    seq_t mult = (run_len < 10) ? (seq_t)10 : (seq_t)100;
    ans = ans * mult + run_len;

    return ans;
}
