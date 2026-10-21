/*
 * A292513  A number N is called "docile" if there are two integers a and b
 *          such that N = a + b with a > b > 0 and S(a) = S(b) where S(n) is
 *          the sum of the digits of the number n.
 * https://oeis.org/A292513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search docile numbers by testing integer pairs a,b with a>b>0
   and equal digit sums.  a(n) is the nth positive integer N that
   can be written as such a sum. */
#include <stdbool.h>

/* sum of decimal digits of x */
static seq_t digit_sum(seq_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* true if N can be expressed as a+b with a>b>0 and digit_sum(a)=digit_sum(b) */
static bool is_docile(seq_t N)
{
    seq_t start = N / 2 + 1;            /* smallest a with a>b>0 */
    for (seq_t a = start; a < N; ++a) {
        seq_t b = N - a;
        if (digit_sum(a) == digit_sum(b))
            return true;
    }
    return false;
}

/* nth docile number (1‑based index) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* out of range, not used in tests */

    seq_t candidate = 11;      /* first possible docile number */
    idx_t count = 0;

    for (;;) {
        if (is_docile(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
