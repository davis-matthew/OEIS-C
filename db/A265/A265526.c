/*
 * A265526  Largest base-2 palindrome m <= n, written in base 2.
 * https://oeis.org/A265526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest binary palindrome m ≤ n, returned as the decimal
 *            integer whose decimal digits are the bits of m. Example: n = 5 →
 *            m = 101₂ = 5₁₀, function returns 101 (decimal)
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"   /* floor_log2_uint64(idx_t) */

/* Reverse the lowest 'bits' bits of x (bits ≤ 64). */
static seq_t reverse_bits(seq_t x, unsigned bits)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Construct a palindrome of total length L from the given high half
   prefix.  L ≥ 1.  The prefix contains ceil(L/2) bits and its
   highest bit is 1 (unless L == 1). */
static seq_t make_palindrome(seq_t prefix, unsigned L)
{
    unsigned half_len = (L + 1) >> 1;   /* ceil(L/2) */
    if ((L & 1) == 0) {                /* even length */
        return (prefix << half_len) | reverse_bits(prefix, half_len);
    } else {                           /* odd length */
        unsigned sh = half_len - 1;    /* number of bits to mirror */
        return (prefix << sh) | reverse_bits(prefix >> 1, sh);
    }
}

/* Return the largest binary palindrome ≤ n, expressed as a decimal
   integer consisting of the bits of that palindrome. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Number of bits in n (n > 0). */
    unsigned L = (unsigned)floor_log2_uint64(n) + 1;

    /* Number of bits in the "high" half (including middle if L odd). */
    unsigned half_len = (L + 1) >> 1;

    /* Extract the high half prefix from n. */
    seq_t prefix = n >> (L - half_len);

    /* Candidate palindrome using the extracted prefix. */
    seq_t pal = make_palindrome(prefix, L);
    if (pal <= n) {
        /* Convert binary palindrome to decimal representation of bits. */
        seq_t result = 0;
        for (int i = (int)L - 1; i >= 0; --i)
            result = result * 10 + ((pal >> i) & 1);
        return result;
    }

    /* Try a smaller prefix if it still has a leading 1. */
    if (prefix > 0) {
        seq_t new_prefix = prefix - 1;
        if (new_prefix >= ((seq_t)1 << (half_len - 1))) {
            pal = make_palindrome(new_prefix, L);
            seq_t result = 0;
            for (int i = (int)L - 1; i >= 0; --i)
                result = result * 10 + ((pal >> i) & 1);
            return result;
        }
    }

    /* Fallback: use the largest palindrome with one fewer bit (all ones). */
    unsigned L2 = L - 1;
    seq_t all_ones = ((seq_t)1 << L2) - 1;
    seq_t result = 0;
    for (int i = (int)L2 - 1; i >= 0; --i)
        result = result * 10 + 1;   /* all bits are 1 */
    return result;
}
