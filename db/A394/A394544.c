/*
 * A394544  Largest prime factor of the n-th base 10 palindrome with
 *          a(1)=a(2)=1.
 * https://oeis.org/A394544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = largest_prime_factor of the n-th base-10 palindrome for n
 *            > 1, with a(1)=a(2)=1 (palindrome 0 and 1)
 * Range:     u8 n <= 35, u16 n <= 800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/largest_prime_factor.h"

static seq_t nth_palindrome(idx_t n)
{
    /* n is 1-based.  Returns the n-th palindrome (0,1,2,3,4,5,6,7,8,9,11,22,33,...) */
    if (n == 1) return 0;
    if (n == 2) return 1;

    idx_t idx = n - 1;            /* zero‑based index */
    idx_t length = 1;
    idx_t cumulative = 10;        /* count of palindromes up to current length */

    /* Find the length that contains the n‑th palindrome */
    while (cumulative <= idx) {
        length ++;
        idx_t half = (length + 1) / 2;
        /* Count for this length: 9 * 10^(half-1) */
        idx_t count = 9 * (idx_t)1;
        for (idx_t i = 1; i < half; ++i) count *= 10;
        cumulative += count;
    }

    idx_t prev_cumulative = cumulative - ((length % 2 == 1) ? 9 * (idx_t)1 : 9 * (idx_t)1)
                            * ((length > 1) ? (idx_t)1 : (idx_t)1); /* adjust if length>1 */
    /* Actually compute previous cumulative manually for correctness */
    prev_cumulative = 0;
    for (idx_t len = 1; len < length; ++len) {
        idx_t half = (len + 1) / 2;
        idx_t count = 10; /* for len==1 */
        if (len > 1) {
            count = 9;
            for (idx_t i = 1; i < half; ++i) count *= 10;
        }
        prev_cumulative += count;
    }
    idx_t offset = idx - prev_cumulative;          /* 0‑based offset within this length */

    /* Construct the palindrome */
    if (length == 1) {
        return (seq_t)(offset); /* 0‑9 */
    }

    idx_t half_len = (length + 1) / 2;
    seq_t base = pow10(half_len - 1);                /* leading digit 1, remaining zeros */
    seq_t left_half = base + (seq_t)offset;           /* first half (and middle if odd) */

    seq_t pal = left_half;
    seq_t tmp = left_half;
    if (length % 2) tmp /= 10;                      /* drop middle digit for odd length */

    while (tmp > 0) {
        pal = pal * (seq_t)10 + (tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    return pal;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1; /* 0 and 1 have largest prime factor 1 */

    seq_t pal = nth_palindrome(n);

    if (pal <= (seq_t)1) return (seq_t)1;

    /* Compute largest prime factor of pal – pal is small enough for uint64_t */
    uint64_t pf = largest_prime_factor((uint64_t)pal);
    return (seq_t)pf;
}
