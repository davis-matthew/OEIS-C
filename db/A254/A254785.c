/*
 * A254785  Zeroless numbers n with digits d_1, d_2, ... d_k such that F(d_1)
 *          + F(d_2) + ... + F(d_k) is a Fibonacci number where F(.) is
 *          A000045.
 * https://oeis.org/A254785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer with no zero digit such that the sum of
 *            Fibonacci values of its decimal digits is a Fibonacci number
 * Range:     u8 n <= 51, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int fib_set[2001];
static int fib_set_done = 0;

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_val = 1;   /* next candidate integer to test */

/* Pre‑compute Fibonacci numbers up to 2000 */
static void init_fib_set(void)
{
    int f0 = 0, f1 = 1;
    fib_set[0] = 1;
    fib_set[1] = 1;
    while (1) {
        int f2 = f0 + f1;
        if (f2 > 2000) break;
        fib_set[f2] = 1;
        f0 = f1;
        f1 = f2;
    }
}

/* Resize the cache when necessary */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = newptr;
        cache_cap = new_cap;
    }
}

/* Test whether candidate integer x satisfies the sequence condition */
static int qualifies(idx_t x)
{
    const int fib_digit[10] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    idx_t tmp = x;
    int sum = 0;
    while (tmp) {
        int d = tmp % 10;
        if (d == 0) return 0;          /* contains zero digit – skip */
        sum += fib_digit[d];
        tmp /= 10;
    }
    return sum <= 2000 && fib_set[sum];
}

/* Main function requested by the compiler */
seq_t a(idx_t n)
{
    if (!fib_set_done) {
        init_fib_set();
        fib_set_done = 1;
    }

    while (cache_len < n) {
        if (qualifies(next_val)) {
            grow_cache();
            cache[cache_len++] = (seq_t)next_val;
        }
        next_val++;
    }

    return cache[n - 1];
}
