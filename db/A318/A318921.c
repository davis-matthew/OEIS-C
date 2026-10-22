/*
 * A318921  In binary expansion of n, delete one symbol from each run. Set
 *          a(n)=0 if the result is the empty string.
 * https://oeis.org/A318921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, hear
 * Formula:   not stated
 * Range:     u8 n <= 1022, u16 n <= 8763, u32 n <= 8763, u64 n <= 8763,
 *            u128 n <= 8763, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A318921: delete one symbol from each run in the binary
    expansion of n.   After removing one bit from each run, the remaining
    bits are concatenated (leading zeros stripped).  If all runs are of
    length 1, the result is the empty string, represented as 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = 0;

    /* number of bits in n (msb position + 1), 64‑bit unsigned */
    unsigned bits = 64 - __builtin_clzll(n);

    unsigned run_bit  = (n >> (bits - 1)) & 1;  /* first bit */
    unsigned run_len  = 0;

    for (int i = bits - 1; i >= 0; --i)
    {
        unsigned bit = (n >> i) & 1;
        if (bit == run_bit)
        {
            ++run_len;
        }
        else
        {
            if (run_len > 1)
            {
                /* output run_len‑1 copies of run_bit */
                for (unsigned k = 0; k < run_len - 1; ++k)
                    res = (res << 1) | (seq_t)run_bit;
            }
            run_bit = bit;
            run_len = 1;
        }
    }

    /* process the final run */
    if (run_len > 1)
    {
        for (unsigned k = 0; k < run_len - 1; ++k)
            res = (res << 1) | (seq_t)run_bit;
    }

    return res;
}
