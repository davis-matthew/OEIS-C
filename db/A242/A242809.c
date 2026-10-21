/*
 * A242809  a(n) is the largest n-digit number whose truncation after its
 *          first k digits is divisible by the k-th Fibonacci number for k =
 *          1..n.
 * https://oeis.org/A242809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation: recursively construct the largest n‑digit number
 * whose prefixes are divisible by the corresponding Fibonacci numbers.
 * For n > 11 no such number exists, the function returns 0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

static seq_t cache[12];
static bool seen[12] = {0};

static bool dfs(idx_t pos, idx_t n, seq_t cur, seq_t *result)
{
    if (pos > n) {
        *result = cur;
        return true;
    }
    seq_t f = fib(pos);          /* Fibonacci number F_pos */
    for (int d = 9; d >= 0; --d) {
        seq_t next = cur * (seq_t)10 + (seq_t)d;
        if (next % f == 0) {
            if (dfs(pos + 1, n, next, result))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 11)   /* no solution for n > 11 */
        return (seq_t)0;

    if (seen[n])
        return cache[n];

    seq_t val = 0;
    bool ok = dfs(1, n, (seq_t)0, &val);
    cache[n] = ok ? val : (seq_t)0;
    seen[n] = true;
    return cache[n];
}
