/*
 * A261095  First differences of A219640: a(n) = A219640(n+1) - A219640(n).
 * https://oeis.org/A261095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A219640(n+1) - A219640(n), where A219640 lists the
 *            distinct values of k - (number of 1's in the Zeckendorf
 *            expansion of k); computed by a direct scan over k
 * Range:     u8 n <= 10946, u16 n <= 10946, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 137 terms through n = 7599 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A261095_fib[92];
static int A261095_fib_n = 0;

static unsigned A261095_zpop(uint64_t k)
{
    unsigned c = 0;
    int i = A261095_fib_n - 1;
    while (k > 0 && i >= 0) {
        if (A261095_fib[i] <= k) {
            k -= A261095_fib[i];
            c++;
            i -= 2;
        } else {
            i--;
        }
    }
    return c;
}

seq_t a(idx_t n)
{
    static unsigned char *diffs = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1, last = 0;

    if (A261095_fib_n == 0) {
        A261095_fib[0] = 1;
        A261095_fib[1] = 2;
        A261095_fib_n = 2;
        while (A261095_fib_n < 90) {
            A261095_fib[A261095_fib_n] = A261095_fib[A261095_fib_n - 1] + A261095_fib[A261095_fib_n - 2];
            A261095_fib_n++;
        }
    }

    while (cnt <= (size_t)n) {
        uint64_t m = next_k - A261095_zpop(next_k);
        next_k++;
        if (m > last) {
            if (cnt >= cap) {
                size_t nc = cap ? cap * 2 : 1024;
                unsigned char *t = (unsigned char *)realloc(diffs, nc);
                if (!t) abort();
                diffs = t;
                cap = nc;
            }
            diffs[cnt++] = (unsigned char)(m - last);
            last = m;
        }
    }
    return (seq_t)diffs[n];
}
