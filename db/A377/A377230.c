/*
 * A377230  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), ... such that for any n >= 0, s(n) = Sum_{k=1..n}
 *          1/(T(k)*a(k)) < 1, T = A000217.
 * https://oeis.org/A377230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy algorithm: maintain the partial sum s = p/q < 1.  
   For the next triangular number T = n(n+1)/2, we need the
   smallest integer a satisfying 1/(T*a) < 1 - p/q.  This gives  
   a = ⌊ q / (T*(q-p)) ⌋ + 1.  The sum is updated by adding the term.  */

#include <stdbool.h>

/* Euclidean gcd for seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but return 0 */

    seq_t p = 0;                   /* numerator of current sum */
    seq_t q = 1;                   /* denominator of current sum */

    seq_t a_val = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* T(k) = k*(k+1)/2 */
        seq_t Tk = ((seq_t)k * (k + 1)) / 2;

        /* bound = floor( q / (T*(q-p)) ); a = bound + 1 */
        seq_t denom_bound = Tk * (q - p);
        seq_t bound = q / denom_bound;
        a_val = bound + 1;

        if (k == n)
            return a_val;

        /* update the partial sum: p/q + 1/(Tk*a_val) */
        seq_t prod = Tk * a_val;            /* Tk*a_val fits in seq_t (arbitrary precision) */
        seq_t new_num = p * prod + q;
        seq_t new_den = q * prod;
        seq_t g = gcd_seq_t(new_num, new_den);
        p = new_num / g;
        q = new_den / g;
    }

    /* unreachable, but keep compiler happy */
    return a_val;
}
