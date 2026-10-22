/*
 * A371238  Euler totient function applied to the binary palindromes of even
 *          length.
 * https://oeis.org/A371238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = φ(A048701(n))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A048701(n) is the n‑th binary palindrome of even length. */

#include <stdint.h>
#include "../utils/seq_phi.h"

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Find k = ceil(log2(n+1)), i.e. the length/2 of the palindrome. */
    idx_t t = n + 1;
    idx_t floor_log = 63 - __builtin_clzll(t);          /* floor(log2(t))   */
    idx_t k = floor_log + ((t & (t - 1)) ? 1 : 0);      /* ceil(log2(t))    */

    /* Offset of n within the block of length‑2k palindromes. */
    idx_t offset = n - ((idx_t)1 << (k - 1));           /* 0 … 2^(k-1)-1    */

    /* First half of the palindrome (k bits). */
    uint64_t half = ((uint64_t)1 << (k - 1)) + (uint64_t)offset;

    /* Reverse the k bits of 'half'. */
    uint64_t rev = 0;
    for (idx_t i = 0; i < k; ++i)
        rev = (rev << 1) | ((half >> i) & 1ULL);

    /* Full even‑length binary palindrome. */
    seq_t p = ((seq_t)half << k) | (seq_t)rev;

    /* Return its Euler totient. */
    return seq_phi(p);
}
