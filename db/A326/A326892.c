/*
 * A326892  a(n) is the reversal of the next composite after a(n-1) if n is
 *          prime, otherwise it is the reversal of the next prime after
 *          a(n-1), starting with a(1) = 1.
 * https://oeis.org/A326892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 9175, u32 n <= 9175, u64 n <= 9175,
 *            u128 n <= 9175, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 1:
     if n is prime:   a(n) = rev( next composite after a(n-1) )
     else            a(n) = rev( next prime    after a(n-1) )
   rev(x) reverses the decimal digits of x. */

#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------- */
/* Simple primality tests for idx_t and seq_t values.              */

static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static bool is_prime_seq_t(seq_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (seq_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* --------------------------------------------------------------- */
/* Helpers to get the next prime/composite after a given value.    */

static seq_t next_prime_seq(seq_t start)
{
    seq_t x = start + 1;
    while (!is_prime_seq_t(x)) ++x;
    return x;
}

static seq_t next_composite_seq(seq_t start)
{
    seq_t x = start + 1;
    while (true) {
        if (x < 4) { x = 4; break; }
        if (!is_prime_seq_t(x)) break;
        ++x;
    }
    return x;
}

/* --------------------------------------------------------------- */
/* Decimal digit reversal.                                         */

static seq_t reverse_digits(seq_t n)
{
    seq_t rev = 0;
    if (n == 0) return 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

/* --------------------------------------------------------------- */
/* Sequence storage – dynamic array with memoisation.              */

static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size               */
static idx_t cache_len = 0;   /* number of terms computed      */

/* --------------------------------------------------------------- */
/* Main sequence function.                                          */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used, safety */

    /* Resize storage if needed. */
    if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else {
            seq_t prev = cache[i - 2];
            if (is_prime_idx(i)) {
                seq_t comp = next_composite_seq(prev);
                val = reverse_digits(comp);
            } else {
                seq_t prime = next_prime_seq(prev);
                val = reverse_digits(prime);
            }
        }
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
