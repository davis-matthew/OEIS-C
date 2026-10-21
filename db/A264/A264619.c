/*
 * A264619  a(0) = 1; for n>0, working in binary, write n followed by 1 then
 *          n-reversed (including leading zeros); show result in base 10.
 * https://oeis.org/A264619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, write the binary representation of n, follow it
 *            with a single 1, then append the binary reversal of n (including
 *            leading zeros to length of n). Interpret the resulting binary
 *            string as a base‑10 integer
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 8419, u64 n <= 8419,
 *            u128 n <= 8419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Determine the number of bits needed to represent n (n>0). */
static inline idx_t bitlen_u64(idx_t n)
{
    idx_t len = 0;
    while (n)
    {
        n >>= 1;
        len++;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */

    /* Number of bits in n. */
    idx_t bits = bitlen_u64(n);

    /* Reverse the bits of n (including leading zeros to length bits). */
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i)
    {
        rev = (rev << 1) | ((n >> i) & 1);
    }

    /* Concatenate: n, then 1, then reversed n. */
    seq_t result = ((seq_t)n << (bits + 1))
                 | ((seq_t)1 << bits)
                 | rev;
    return result;
}
