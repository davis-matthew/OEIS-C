/*
 * A358693  Numbers k such that k / (sum of digits of k) is the square of a
 *          prime.
 * https://oeis.org/A358693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k such that k /
 *            (sum_of_digits(k)) is a perfect square of a prime
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: bit length of an unsigned value (generic for any seq_t) */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        len++;
    }
    return len;
}

/* Helper: floor square root of an unsigned value (generic for any seq_t) */
static seq_t floor_sqrt_seq_t(seq_t x)
{
    seq_t res = 0;
    /* largest power of two <= sqrt(x) */
    unsigned len = bitlen_seq_t(x);
    seq_t bit = (seq_t)1 << ((len + 1) / 2 * 2);
    while (bit) {
        seq_t trial = res | bit;
        if (trial * trial <= x) {
            res = trial;
        }
        bit >>= 1;
    }
    return res;
}

/* Helper: simple primality test for seq_t (trial division) */
static bool is_prime_generic(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    seq_t limit = floor_sqrt_seq_t(n);
    for (seq_t d = 3; d <= limit; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Helper: sum of decimal digits of an unsigned value */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* The required sequence generator with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* below offset – undefined in OEIS */
        return (seq_t)0;
    }

    /* static cache: values found so far */
    static seq_t *cache = NULL;
    static size_t sz    = 0;      /* number of cached elements */
    static size_t cap   = 0;      /* allocated capacity */
    static seq_t cur    = 1;      /* current integer being examined */

    while (sz < n) {
        ++cur;                    /* next integer to test */
        seq_t sum = digit_sum(cur);                /* decimal digit sum */
        if (cur % sum == 0) {                       /* divisibility holds */
            seq_t q = cur / sum;                    /* quotient k / sumdigits */
            seq_t r = floor_sqrt_seq_t(q);          /* floor sqrt of q */
            if (r * r == q && is_prime_generic(r)) {/* perfect square of a prime */
                /* grow cache if needed */
                if (sz == cap) {
                    size_t newcap = cap ? cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) {
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                    cap   = newcap;
                }
                cache[sz++] = cur;                 /* store found term */
            }
        }
    }
    return cache[n - 1];
}
