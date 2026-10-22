/*
 * A303536  Number of terms in greedy partition of n into binary palindromes.
 * https://oeis.org/A303536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of terms in the greedy partition of n into binary palindromes.
 * The algorithm: repeatedly subtract the largest binary palindrome ≤ current value
 * until zero is reached, counting the steps.
 */

#include <stdint.h>

static inline idx_t floor_log2_uint64(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* Construct a binary palindrome of given length with the supplied left half (prefix).
   For odd lengths the prefix includes the middle bit. */
static inline seq_t make_pal(seq_t prefix, idx_t len)
{
    idx_t half = (len + 1) / 2;                /* ceil(len/2) */
    seq_t rev  = 0;
    seq_t tmp;

    if (len & 1) { /* odd length */
        tmp = prefix >> 1;                    /* drop the middle bit */
        for (idx_t i = 0; i < half - 1; ++i) {
            rev = (rev << 1) | (tmp & 1);
            tmp >>= 1;
        }
        return (prefix << (half - 1)) | rev;
    } else { /* even length */
        tmp = prefix;
        for (idx_t i = 0; i < half; ++i) {
            rev = (rev << 1) | (tmp & 1);
            tmp >>= 1;
        }
        return (prefix << half) | rev;
    }
}

/* Largest binary palindrome <= n (n > 0).  The result is an idx_t. */
static inline idx_t largest_pal(idx_t n)
{
    if (n == 0) return 0;

    idx_t len  = floor_log2_uint64(n) + 1;      /* bit length of n */
    idx_t half = (len + 1) / 2;                 /* ceil(len/2) */

    seq_t prefix = ((seq_t)n) >> (len - half);  /* top half bits (as seq_t) */

    seq_t cand = make_pal(prefix, len);
    if (cand <= n) return (idx_t)cand;

    seq_t prefix_dec = prefix - 1;
    /* If decrementing removes the leading 1, the new palindrome has length len-1 */
    if (prefix_dec < ((seq_t)1 << (half - 1))) {
        return ((idx_t)((((seq_t)1 << (len - 1)) - 1)));
    } else {
        return (idx_t)make_pal(prefix_dec, len);
    }
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t cur = n;

    while (cur) {
        cnt++;
        idx_t pal = largest_pal(cur);
        cur -= pal;               /* subtract the palindrome */
    }
    return cnt;
}
