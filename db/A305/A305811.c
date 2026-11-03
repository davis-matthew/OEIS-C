/*
 * A305811  Filter sequence for a(Fibonacci prime) = constant sequences.
 * https://oeis.org/A305811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=2 if n is a Fibonacci prime; otherwise
 *            a(n)=n - (number of Fibonacci primes ≤ n) + 1
 * Range:     u8 n <= 259, u16 n <= 65541, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 110 terms through n = 9391 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: test whether a given value is a Fibonacci prime. */
static bool is_fibonacci_prime(seq_t x)
{
    if (x < 2)
        return false;
    seq_t a = 0, b = 1;           /* Fibonacci numbers F(0)=0, F(1)=1 */
    while (b <= x) {
        if (b == x)
            return is_prime_big(b);   /* b is Fibonacci; check primality */
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* offset 1 */
        return (seq_t)1;

    seq_t sn = (seq_t)n;

    /* Special value for Fibonacci primes */
    if (is_fibonacci_prime(sn))
        return (seq_t)2;

    /* Count Fibonacci primes ≤ n */
    seq_t count = 0;
    seq_t a_fib = 0, b_fib = 1;   /* same generation of Fibonacci numbers */
    while (b_fib <= sn) {
        if (b_fib >= 2 && is_prime_big(b_fib))
            ++count;
        seq_t tmp = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = tmp;
    }

    /* Running count for non‑Fibonacci‑prime indices, starting at 3 */
    return sn - count + 1;
}
