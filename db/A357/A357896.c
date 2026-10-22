/*
 * A357896  Additive triprimes.
 * https://oeis.org/A357896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357896: additive triprimes.
   a(n) is the n‑th integer ≥ 8 that is a product of exactly
   three primes (with multiplicity) and whose decimal digit sum
   is also a product of exactly three primes.
   The code computes a(n) by a simple search using trial division. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- auxiliary: integer square‑root (floor) ------------------ */
static uint64_t floor_sqrt(uint64_t n)
{
    if (n < 2) return n;
    uint64_t lo = 1, hi = n >> 1, ans = 1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) { lo = mid + 1; ans = mid; }
        else        { hi = mid - 1; }
    }
    return ans;
}

/* ----------------- prime list management --------------------------- */
static uint64_t *prime_list   = NULL;        /* dynamically allocated primes */
static size_t   prime_cnt    = 0;             /* number of stored primes  */
static size_t   prime_cap    = 0;             /* allocated capacity     */
static uint64_t prime_next   = 3;             /* next odd number to test */

/* expand the prime list so that all primes ≤ limit are stored */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_list == NULL) {
        /* initial allocation: store at least the prime 2 */
        prime_cap  = 32;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (prime_list == NULL) return; /* allocation failure – give up */
        prime_list[0] = 2;
        prime_cnt = 1;
    }
    while (prime_next <= limit) {
        bool is_prime = true;
        uint64_t r = floor_sqrt(prime_next);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (prime_next % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 32 : prime_cap << 1;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (prime_list == NULL) return;
            }
            prime_list[prime_cnt++] = prime_next;
        }
        prime_next += 2;
    }
}

/* -------------- test whether n is a product of exactly three primes */
static bool is_product_of_three_primes(uint64_t n)
{
    if (n < 2) return false;
    int cnt = 0;
    uint64_t m = n;
    uint64_t limit = floor_sqrt(m);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        while (m % p == 0) {
            ++cnt;
            m /= p;
            if (cnt > 3) return false; /* early exit */
        }
        limit = floor_sqrt(m);
    }
    if (m > 1) ++cnt;
    return cnt == 3;
}

/* -------------- decimal digit sum ---------------------------------- */
static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* ---------------- public sequence function ----------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t count = 0;
    uint64_t num   = 8;          /* first possible additive triprime */

    while (true) {
        if (is_product_of_three_primes(num)) {
            uint64_t sum = digit_sum_uint64(num);
            if (is_product_of_three_primes(sum)) {
                ++count;
                if (count == n) return (seq_t)num;
            }
        }
        ++num;
    }
}
