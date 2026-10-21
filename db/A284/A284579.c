/*
 * A284579  Carryless base-2 product (A048720) of run lengths in binary
 *          representation of n.
 * https://oeis.org/A284579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless base‑2 product of run lengths in binary
 *            representation of n
 * Range:     u8 n <= 9799, u16 n <= 9799, u32 n <= 9799, u64 n <= 9799,
 *            u128 n <= 9799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute carryless (polynomial) product of two integers in binary.
   The product is defined over GF(2); it is equivalent to XORing
   all bit‑shifted copies of the second operand, one for each set bit
   of the first operand. */
static inline seq_t carryless_mul(seq_t a, seq_t b)
{
    seq_t res = 0;
    uint64_t pos = 0;
    while (a)
    {
        if (a & (seq_t)1)
            res ^= (b << pos);
        a >>= 1;
        pos++;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 has binary representation "0",
       run length list = [1], so the product is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Scan bits of n from LSB to MSB, collecting run lengths. */
    seq_t result = (seq_t)1;          /* multiplicative identity for carryless product */
    int curbit = (int)(n & 1);        /* current bit value */
    seq_t runlen = 1;                 /* length of the current run */
    idx_t tmp = n >> 1;

    while (tmp)
    {
        int bit = (int)(tmp & 1);
        if (bit == curbit)
        {
            runlen++;                /* extend current run */
        }
        else
        {
            /* Finish the previous run: multiply result by its length. */
            result = carryless_mul(result, (seq_t)runlen);
            curbit = bit;           /* start new run */
            runlen = 1;
        }
        tmp >>= 1;
    }

    /* Final run */
    result = carryless_mul(result, (seq_t)runlen);
    return result;
}
