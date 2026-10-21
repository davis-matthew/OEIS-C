/*
 * A258649  Ninth arithmetic derivative of n.
 * https://oeis.org/A258649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003415)^9(n) Ninth iterate of the arithmetic derivative
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 639, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PRIME_LIMIT 100000
static int primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool sieve_done = false;

static void sieve_primes(void)
{
    if (sieve_done) return;
    bool is_composite[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_composite[i] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = true;
    prime_count = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) primes[prime_count++] = (int)i;
    sieve_done = true;
}

static void factorise(seq_t n, uint64_t p_arr[], unsigned e_arr[], size_t *cnt)
{
    sieve_primes();
    seq_t tmp = n;
    size_t c = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        int p = primes[i];
        seq_t sp = (seq_t)p;
        if (sp * sp > tmp) break;
        unsigned e = 0;
        while (tmp % sp == 0) {
            tmp /= sp;
            ++e;
        }
        if (e) {
            p_arr[c] = p;
            e_arr[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {
        p_arr[c]   = (uint64_t)tmp;
        e_arr[c]   = 1;
        ++c;
    }
    *cnt = c;
}

static seq_t arithmetic_derivative(seq_t n)
{
    if (n <= 1) return 0;
    uint64_t p[20];
    unsigned e[20];
    size_t cnt;
    factorise(n, p, e, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (n / (seq_t)p[i]) * e[i];
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    for (int i = 0; i < 9; ++i) {
        cur = arithmetic_derivative(cur);
    }
    return cur;
}
