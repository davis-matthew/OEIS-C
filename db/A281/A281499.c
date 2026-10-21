/*
 * A281499  Write n in binary reflected Gray code, interchange the 1's and
 *          0's, reverse the code and convert it back to decimal.
 * https://oeis.org/A281499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A036044(A003188(n)) The function computes the sequence
 *            A281499: write n in binary reflected Gray code, flip bits,
 *            reverse, convert to decimal
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute bit length of a non‑negative idx_t number.
   For n = 0 the length is 1 to allow a 1‑bit representation. */
static inline idx_t bitlen(idx_t n)
{
    if (n == 0) return 1;
    idx_t len = 0;
    while (n) {
        ++len;
        n >>= 1;
    }
    return len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1. Gray code of n. */
    idx_t g = n ^ (n >> 1);

    /* 2. Determine the bit width to use. */
    idx_t L = bitlen(n);

    /* 3. Build a mask for the lower L bits. */
    seq_t mask;
    /* If seq_t is 64 bits wide and L == 64 we must avoid shifting by 64
       (undefined for 64‑bit types). In that case all bits set in seq_t are
       the correct mask, because only the lower 64 bits will be used later. */
    if (sizeof(seq_t) <= sizeof(uint64_t) && L == 64)
        mask = ~((seq_t)0);
    else
        mask = ((seq_t)1 << L) - 1;

    /* 4. Complement the Gray code within the L bits. */
    seq_t comp = (~((seq_t)g)) & mask;

    /* 5. Reverse the L bits. */
    seq_t rev = 0;
    for (idx_t i = 0; i < L; ++i) {
        if (comp & ((seq_t)1 << i))
            rev |= ((seq_t)1 << (L - 1 - i));
    }
    return rev;
}
