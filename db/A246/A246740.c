/*
 * A246740  Number of length 3+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = 3*a(n-1) + 2*a(n-2) - 14*a(n-3) + 5*a(n-4) +
 *            25*a(n-5) - 20*a(n-6) - 20*a(n-7) + 25*a(n-8) + 5*a(n-9) -
 *            14*a(n-10) + 2*a(n-11) + 3*a(n-12) - a(n-13) a(1)=2, a(2)=22,
 *            a(3)=548, a(4)=2884, a(5)=18966, a(6)=66378, a(7)=241544,
 *            a(8)=643048, a(9)=1687850, a(10)=3716830, a(11)=7980972...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Use a static cache large enough for all n used in the tests
   (about 10 000).  It works for both C11 and C++17: a static
   array of plain integers or of an arbitrary‑precision type is
   constructed once and never moved. */
#define MAXCACHE 20000
static seq_t cache[MAXCACHE + 1];
static idx_t last = 0;

seq_t a(idx_t n)
{
    if (n < 1 || n > MAXCACHE)
        return (seq_t)0;          /* out of supported range */

    /* initialise the first 13 terms once */
    if (last < 13) {
        static const seq_t init[14] = {
            0ULL, /* unused index 0 */
            2ULL, 22ULL, 548ULL, 2884ULL, 18966ULL,
            66378ULL, 241544ULL, 643048ULL, 1687850ULL,
            3716830ULL, 7980972ULL, 15368172ULL, 28868798ULL
        };
        idx_t upto = (n < 13 ? n : 13);
        for (idx_t i = last + 1; i <= upto; ++i)
            cache[i] = init[i];
        last = upto;
    }

    /* compute further terms if needed */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t pos = cache[i - 1] * (seq_t)3
                      + cache[i - 2] * (seq_t)2
                      + cache[i - 4] * (seq_t)5
                      + cache[i - 5] * (seq_t)25
                      + cache[i - 8] * (seq_t)25
                      + cache[i - 9] * (seq_t)5
                      + cache[i - 11] * (seq_t)2
                      + cache[i - 12] * (seq_t)3;
            seq_t neg = cache[i - 3] * (seq_t)14
                      + cache[i - 6] * (seq_t)20
                      + cache[i - 7] * (seq_t)20
                      + cache[i - 10] * (seq_t)14
                      + cache[i - 13];
            cache[i] = pos - neg;   /* pos >= neg, no wrap‑around */
        }
        last = n;
    }
    return cache[n];
}
