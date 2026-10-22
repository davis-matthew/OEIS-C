/*
 * A357774  Binary expansions of odd numbers with two zeros in their binary
 *          expansion.
 * https://oeis.org/A357774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal number whose digits are the bits of the nth
 *            odd integer whose binary expansion contains exactly two zeroes,
 *            sorted in increasing order
 * Range:     u8 none, u16 n <= 4, u32 n <= 84, u64 n <= 969, u128 n <= 8436,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* C(x,3) for 0 ≤ x ≤ ~6_000_000_000_000_000_000 (fits in 128 bits) */
static inline idx_t choose3(idx_t x)
{
    if (x < 3) return 0;
    unsigned __int128 r = (unsigned __int128)x * (x - 1) * (x - 2);
    r /= 6;
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    /* 1. Find the bit length m of the desired number. */
    idx_t m = 4;
    while (choose3(m - 1) < n) ++m;

    /* 2. Find local position within the block of length‑m numbers. */
    idx_t prev_total = choose3(m - 2);   /* C(m-2,3)  */
    idx_t local_k   = n - prev_total;    /* 1‑based index in this block */

    /* 3. Length of the middle part (bits except the leading and trailing 1s). */
    idx_t L = m - 2;  /* L ≥ 2 for m ≥ 4 */

    /* 4. Locate the two zero positions among the L middle bits.
       They are identified by indices 0 ≤ i < j < L, counted from
       the most significant middle bit (0) to the least significant (L‑1). */
    idx_t zero_a = 0, zero_b = 1;          /* dummy init */
    idx_t count  = local_k;                /* remaining pair to skip */
    for (idx_t i = 0; i < L - 1; ++i) {
        for (idx_t j = i + 1; j < L; ++j) {
            if (--count == 0) {
                zero_a = i;
                zero_b = j;
                goto found;
            }
        }
    }
found:

    /* 5. Construct the decimal representation digit by digit.
       The number starts with a leading 1,
       then the L middle bits, then a trailing 1. */
    seq_t result = 1;        /* leading '1' */
    for (idx_t pos = 0; pos < L; ++pos) {
        seq_t bit = (pos == zero_a || pos == zero_b) ? 0 : 1;
        result = result * 10 + bit;
    }
    result = result * 10 + 1;         /* trailing '1' */

    return result;
}
