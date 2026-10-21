/*
 * A279911  a(n) = Sum_{k=0..n} k / gcd(n^k, k).
 * https://oeis.org/A279911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k / gcd(n^k, k) * = Sum_{k=1..n} product of
 *            prime powers of k that do NOT * divide n (i.e. the largest
 *            divisor of k coprime to n)
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool primes_computed = false;
static idx_t primes_arr[1229];
static size_t primes_cnt = 0;

/* Simple sieve up to 10,000, executed once. */
static void init_primes(void)
{
    if (primes_computed)
        return;

    const idx_t LIMIT = 10000;
    bool chk[LIMIT + 1];
    for (idx_t i = 0; i <= LIMIT; ++i)
        chk[i] = true;
    chk[0] = chk[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (chk[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                chk[j] = false;
        }
    }
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (chk[i])
            primes_arr[primes_cnt++] = i;
    }
    primes_computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    init_primes();

    /* factor n into distinct primes */
    static idx_t n_primes[20];
    size_t np_cnt = 0;
    seq_t tmp = (seq_t)n;
    for (size_t i = 0; i < primes_cnt && primes_arr[i] <= n; ++i) {
        idx_t p = primes_arr[i];
        if (tmp % p == 0) {
            n_primes[np_cnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor of n (> sqrt(n)) */
        n_primes[np_cnt++] = (idx_t)tmp;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)k;
        for (size_t j = 0; j < np_cnt; ++j) {
            idx_t p = n_primes[j];
            while (term % p == 0)
                term /= p;
        }
        sum += term;
    }
    return sum;
}
