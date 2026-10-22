/*
 * A342525  Heinz numbers of integer partitions with strictly decreasing first
 *          quotients.
 * https://oeis.org/A342525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose prime indices have
 *            strictly decreasing first quotients. Method: test successive
 *            integers by factorising them into primes, convert each prime to
 *            its 1‑based index, and check that for every three consecutive
 *            indices a, b, c in the ascending sequence the inequal...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *prime_tbl   = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t prime_last  = 1;

/* Ensure storage for at least `need` primes. */
static void ensure_prime_storage(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_tbl, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* Generate all primes up to `limit` and store them in prime_tbl. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_last) return;
    for (uint64_t n = prime_last + 1; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_storage(prime_cnt + 1);
            prime_tbl[prime_cnt++] = (uint32_t)n;
        }
    }
    prime_last = limit;
}

/* Return the 1‑based index of prime `p` in prime_tbl. */
static uint32_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_tbl[mid] == p) return (uint32_t)(mid + 1);
        if (prime_tbl[mid] < p)  lo = mid + 1;
        else                     hi = mid;
    }
    abort(); /* should never happen */
    return 0;
}

/* Test whether integer `n` satisfies the sequence condition. */
static bool good_number(idx_t n)
{
    if (n <= 2) return true;           /* 1 and 2 are always good */
    uint32_t indices[64];
    size_t   len = 0;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t p = 0;
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= m; ++i) {
            if (m % prime_tbl[i] == 0) { p = prime_tbl[i]; break; }
        }
        if (p == 0) {                   /* remaining part is a prime > sqrt(m) */
            ensure_primes_up_to(m);
            p = m;
        }
        uint32_t idx = prime_index(p);
        while (m % p == 0) {
            m /= p;
            indices[len++] = idx;
        }
    }
    if (len <= 2) return true;         /* no triple to violate the condition */
    for (size_t j = 2; j < len; ++j) {
        uint64_t a = indices[j];
        uint64_t b = indices[j - 1];
        uint64_t c = indices[j - 2];
        if (a * c >= b * b) return false;
    }
    return true;
}

/* Public sequence function: a(n) */
seq_t a(idx_t n)
{
    if (prime_cnt == 0) {
        ensure_prime_storage(1);
        prime_tbl[0] = 2;
        prime_cnt = 1;
        prime_last = 2;
    }
    idx_t cur    = 1;
    idx_t count  = 0;
    while (true) {
        if (good_number(cur)) {
            ++count;
            if (count == n) return (seq_t)cur;
        }
        ++cur;
    }
}
