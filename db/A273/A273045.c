/*
 * A273045  Fibonacci numbers with digits in nondecreasing order.
 * https://oeis.org/A273045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A273045:
   Fibonacci numbers with digits in nondecreasing order.
   It iteratively generates Fibonacci numbers, keeps those whose
   decimal representation is nondecreasing, and returns the nth
   such number (the sequence is finite – last term is 377). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool digits_nondecreasing(unsigned long long v)
{
    if (v < 10ULL) return true;
    unsigned long long last = v % 10ULL;
    v /= 10ULL;
    while (v > 0ULL) {
        unsigned long long d = v % 10ULL;
        if (d > last) return false;
        last = d;
        v /= 10ULL;
    }
    return true;
}

static seq_t cache[14];
static size_t cache_count = 0;
static bool built = false;

static void build_cache(void)
{
    if (built) return;
    built = true;

    unsigned long long a = 0ULL, b = 1ULL;  /* a = fib(i), b = fib(i+1) */
    size_t cnt = 0;
    for (unsigned long long i = 0; cnt < 14; ++i) {
        unsigned long long val = a;
        if (digits_nondecreasing(val)) {
            cache[cnt] = (seq_t)val;
            ++cnt;
        }
        unsigned long long next = a + b;
        a = b;
        b = next;
    }
    cache_count = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */
    build_cache();
    if ((size_t)n <= cache_count) {
        return cache[n - 1];
    } else {
        return 0;                  /* sequence ends for this build */
    }
}
