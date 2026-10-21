/*
 * A284581  a(n) = n minus (carryless base-2 product of run lengths in binary
 *          representation of n) = n - A284579(n).
 * https://oeis.org/A284581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A284579(n)
 * Range:     u8 n <= 260, u16 n <= 9625, u32 n <= 9625, u64 n <= 9625,
 *            u128 n <= 9625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A284579(n) = carryless base‑2 product of the lengths of all runs
   of consecutive 0s and 1s in the binary representation of n. */

#include <stdint.h>

/* Carry‑less (polynomial) product over GF(2).  */
static inline seq_t carryless_mul(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (a) {
        if (a & 1)
            res ^= b;
        a >>= 1;
        b <<= 1;
    }
    return res;
}

/* Computes the carryless product of all run lengths (of 0s and 1s)
   in the binary representation of n (n ≥ 1).  */
static seq_t run_length_product(idx_t n)
{
    seq_t prod = 1;          /* multiplicative identity */
    if (n == 0) return 0;    /* not needed for offset ≥1, but defensive */

    /* Find the position of the most significant bit (0‑based). */
    idx_t tmp = n;
    int msb = 0;
    while (tmp >>= 1)
        msb++;

    /* Scan bits from the most significant to the least. */
    int run_len = 1;
    unsigned cur_bit = (n >> msb) & 1;

    for (int i = msb - 1; i >= 0; --i) {
        unsigned bit = (n >> i) & 1;
        if (bit == cur_bit) {
            ++run_len;
        } else {
            prod = carryless_mul(prod, (seq_t)run_len);
            cur_bit = bit;
            run_len = 1;
        }
    }
    /* Last run. */
    prod = carryless_mul(prod, (seq_t)run_len);
    return prod;
}

/* Main sequence function: a(n) = n - carryless product of run lengths.  */
seq_t a(idx_t n)
{
    seq_t prod = run_length_product(n);
    return (seq_t)n - prod;
}
