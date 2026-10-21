/*
 * A299998  Decimal expansion of Sum_{k>=0} 1/2^(k*(k+1)/2).
 * https://oeis.org/A299998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of 1 + Σ_{k>=1} 1/2^{k*(k+1)/2} (a(1) = 1
 *            is the integer part, a(n) for n>=2 is the (n‑1)th decimal digit
 *            after the point)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#define MAX_WORDS 700          /* supports up to ~ 64 * 700 ≈ 44800 bits of precision */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but guard against underflow */
    if (n == 1) return (seq_t)1; /* the integer part */

    /* number of decimal digits to be produced */
    idx_t n_digits = n - 1;

    /* Safety margin: use 10 extra bits to guard against carry propagation. */
    static const double LOG2_10 = 3.32192809488736234787;
    double needed_bits = (double)n_digits * LOG2_10;   /* bits needed for n_digits decimal places */
    idx_t B = (idx_t)ceil(needed_bits) + 10;          /* bit length of the denominator */

    idx_t shift_words = B / 64;           /* how many full 64‑bit words */ 
    idx_t shift_bits  = B % 64;           /* offset within the high word (0…63) */

    /* Ensure we have room for intermediate carries. */
    idx_t words = shift_words + 3;        /* enough for the product by 10 */
    if (words > MAX_WORDS) words = MAX_WORDS;

    /* Initialise remainder array to zero. */
    uint64_t rem[MAX_WORDS];
    memset(rem, 0, words * sizeof(uint64_t));

    /* Set bits for all triangular numbers T_k = k*(k+1)/2 ≤ B. */
    for (idx_t k = 0; ; ++k) {
        idx_t Tk = k * (k + 1) / 2;          /* triangular number */
        if (Tk > B) break;
        idx_t pos = B - Tk;                 /* bit position for this term */
        idx_t wi  = pos / 64;
        idx_t bi  = pos % 64;
        rem[wi] |= (1ULL << bi);
    }

    /* Remove the integer part (bit at position B). */
    if (shift_bits == 0)
        rem[shift_words] -= 1ULL;
    else
        rem[shift_words] -= (1ULL << shift_bits);

    /* Mask to keep only the lower B bits after each extraction. */
    uint64_t low_mask = (shift_bits == 0) ? 0ULL : ((1ULL << shift_bits) - 1ULL);

    seq_t digit = 0;
    for (idx_t step = 0; step < n_digits; ++step) {
        /* Multiply remainder by 10 (big‑integer multiply). */
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < (size_t)words; ++j) {
            unsigned __int128 prod = (unsigned __int128)rem[j] * 10 + carry;
            rem[j] = (uint64_t)prod;
            carry  = prod >> 64;
        }
        /* any leftover carry fits into the next word if present */
        if (carry && words < MAX_WORDS) rem[words] = (uint64_t)carry;

        /* Determine the digit: floor(rem / 2^B). */
        if (shift_bits <= 60) {
            /* All relevant bits lie inside rem[shift_words] */
            digit = (rem[shift_words] >> shift_bits) & 0xF;
        } else {
            /* When shift_bits > 60, we need bits from the next word. */
            uint64_t lo  = rem[shift_words] >> shift_bits;
            uint64_t hi  = rem[shift_words + 1] << (64 - shift_bits);
            digit = (lo | hi) & 0xF;
        }

        /* Keep only the lower B bits for the next iteration. */
        if (shift_bits == 0) {
            rem[shift_words] = 0;
        } else {
            rem[shift_words] &= low_mask;
        }
        for (size_t j = shift_words + 1; j < (size_t)words; ++j) {
            rem[j] = 0;
        }
    }
    return digit;
}
