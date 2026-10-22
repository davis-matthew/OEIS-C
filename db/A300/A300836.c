/*
 * A300836  a(n) is the total number of terms (1-digits) in Zeckendorf
 *          representation of all proper divisors of n.
 * https://oeis.org/A300836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d<n} A007895(d) where A007895(d) = the number of
 *            1‑digits in the Zeckendorf representation of d. OEIS offset: 1
 *            (a(1)=0)
 * Range:     u8 n <= 9479, u16 n <= 9479, u32 n <= 9479, u64 n <= 9479,
 *            u128 n <= 9479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of Fibonacci numbers (F1=1, F2=2, F3=3, …).  */
static idx_t *fib = NULL;
static idx_t fib_len = 0;

/* Grow fib array until it contains a Fibonacci number > limit. */
static void ensure_fib_upto(idx_t limit)
{
    /* initialise first two Fibonacci numbers */
    if (fib_len == 0) {
        fib = (idx_t*)malloc(2 * sizeof(idx_t));
        fib[0] = 1;          /* F1 */
        fib[1] = 2;          /* F2 */
        fib_len = 2;
    }

    while (fib[fib_len - 1] <= limit) {
        idx_t next = fib[fib_len - 1] + fib[fib_len - 2];
        fib = (idx_t*)realloc(fib, (fib_len + 1) * sizeof(idx_t));
        fib[fib_len] = next;
        fib_len++;
    }
}

/* Count the 1‑digits in the Zeckendorf representation of val. */
static seq_t count_ones(idx_t val)
{
    idx_t rem = val;
    seq_t cnt = 0;

    /* work from the largest Fibonacci number downwards */
    for (idx_t i = fib_len; i-- > 0; ) {
        if (rem >= fib[i]) {
            rem -= fib[i];
            cnt++;
        }
        if (rem == 0)
            break;
    }
    return cnt;
}

/* Integer square root (floor) of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t x)
{
    if (x < 2)
        return x;

    idx_t lo = 1, hi = x >> 1, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid) {
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
    if (n <= 1)
        return 0;   /* a(1) = 0, a(0) undefined but never queried */

    /* ensure Fibonacci numbers up to n‑1 are available */
    ensure_fib_upto(n - 1);

    seq_t total = 0;
    idx_t d1, d2;
    idx_t limit = isqrt_u64(n);

    for (idx_t i = 1; i <= limit; i++) {
        if (n % i == 0) {
            d1 = i;
            if (d1 < n)
                total += count_ones(d1);

            d2 = n / i;
            if (d2 != d1 && d2 < n)
                total += count_ones(d2);
        }
    }
    return total;
}
