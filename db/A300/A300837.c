/*
 * A300837  a(n) is the total number of terms (1-digits) in Zeckendorf
 *          representation of all divisors of n.
 * https://oeis.org/A300837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A007895(d), where A007895(d) counts the number
 *            of 1‑digits in the Zeckendorf representation of d
 * Range:     u8 n <= 9218, u16 n <= 9218, u32 n <= 9218, u64 n <= 9218,
 *            u128 n <= 9218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Fibonacci cache: 1, 2, 3, 5, … */
static idx_t *fib_arr = NULL;
static size_t fib_cnt = 0;

/* Ensure fib_arr contains all Fibonacci numbers >= n. */
static void ensure_fib(idx_t n)
{
    if (fib_cnt == 0) {
        fib_cnt = 2;
        fib_arr = (idx_t*)realloc(fib_arr, fib_cnt * sizeof(idx_t));
        if (!fib_arr) abort();
        fib_arr[0] = 1;
        fib_arr[1] = 2;
    }
    while (fib_arr[fib_cnt - 1] < n) {
        fib_arr = (idx_t*)realloc(fib_arr, (fib_cnt + 1) * sizeof(idx_t));
        if (!fib_arr) abort();
        fib_arr[fib_cnt] = fib_arr[fib_cnt - 1] + fib_arr[fib_cnt - 2];
        ++fib_cnt;
    }
}

/* Count 1‑digits in the Zeckendorf representation of x. */
static seq_t zeck_count(idx_t x)
{
    if (x == 0) return 0;
    ensure_fib(x);
    seq_t cnt = 0;
    idx_t rem = x;
    for (int i = (int)fib_cnt - 1; i >= 0; --i) {
        if (fib_arr[i] <= rem) {
            rem -= fib_arr[i];
            ++cnt;
            i -= 1;        /* skip the next Fibonacci number */
        }
    }
    return cnt;
}

/* Recursive enumeration of all divisors and summing Zeckendorf counts. */
static seq_t sum_zeck_divisors(idx_t cur, size_t pos,
                               const idx_t *primes,
                               const uint8_t *exps,
                               size_t pf_cnt)
{
    if (pos == pf_cnt) {
        return zeck_count(cur);
    }
    seq_t total = 0;
    idx_t mult = 1;
    for (uint8_t e = 0; e <= exps[pos]; ++e) {
        total += sum_zeck_divisors(cur * mult, pos + 1, primes, exps,
                                   pf_cnt);
        mult *= primes[pos];
    }
    return total;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out-of‑offset calls */

    /* Factor n into prime powers. */
    idx_t primes[64];
    uint8_t exps[64];
    size_t pf_cnt = 0;
    idx_t m = n;

    if (m % 2 == 0) {
        uint8_t e = 0;
        while (m % 2 == 0) { m /= 2; ++e; }
        primes[pf_cnt] = 2;
        exps[pf_cnt] = e;
        ++pf_cnt;
    }

    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint8_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }

    if (m > 1) {
        primes[pf_cnt] = m;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    return sum_zeck_divisors(1, 0, primes, exps, pf_cnt);
}
