/*
 * A332268  a(n) is the number of divisors of n that are Niven numbers.
 * https://oeis.org/A332268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of divisors of n that are Niven numbers
 * Range:     u8 n <= 2011, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether x is a Niven (Harshad) number. */
static bool is_niven(seq_t x)
{
    if (x == (seq_t)0) return false; /* 0 is not considered a divisor here */
    seq_t temp = x;
    seq_t sum  = 0;
    const seq_t ten = (seq_t)10;

    while (temp > (seq_t)0) {
        sum += temp % ten;   /* add the least significant decimal digit */
        temp /= ten;         /* remove that digit */
    }
    return x % sum == (seq_t)0;
}

/* Integer square root: floor(sqrt(n)).  Safe for uint64_t values. */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    idx_t ans = 0;

    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {            /* guard against 0*0 overflow */
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid > n / mid) {       /* mid*mid > n, use division to avoid overflow */
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive: OEIS index starts at 1 */

    idx_t limit = int_sqrt(n);
    seq_t count = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            if (is_niven((seq_t)i)) ++count;

            idx_t j = n / i;       /* complementary divisor */
            if (j != i && is_niven((seq_t)j)) ++count;
        }
    }
    return count;
}
