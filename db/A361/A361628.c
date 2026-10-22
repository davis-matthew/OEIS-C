/*
 * A361628  Sphenic numbers (products of 3 distinct primes) whose digits are
 *          primes.
 * https://oeis.org/A361628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate numbers with digits 2,3,5,7 and test the
   sphenic (3 distinct prime factors, square‑free) property. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint8_t DIGITS[4] = {2,3,5,7};
static const uint64_t PRIME_LIMIT = 1000000;   /* enough for n≤10000 */

static uint64_t *prime_list = NULL;
static size_t prime_cnt     = 0;

/* Simple sieve of Eratosthenes, generates primes up to PRIME_LIMIT */
static void init_primes(void) {
    if (prime_list != NULL) return;
    uint64_t limit = PRIME_LIMIT;
    bool *composite = (bool *)malloc((limit + 1) * sizeof(bool));
    composite[0] = composite[1] = true;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!composite[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                composite[j] = true;
        }
    }
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!composite[i]) ++cnt;
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!composite[i]) prime_list[idx++] = i;
    prime_cnt = cnt;
    free(composite);
}

/* Test whether x is a sphenic number (product of 3 distinct primes) */
static bool is_sphenic(uint64_t x) {
    if (x < 30) return false;            /* 2*3*5 = 30 is the smallest */
    uint64_t tmp = x;
    size_t distinct = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            ++distinct;
            tmp /= p;
            if (tmp % p == 0) return false;   /* squared prime factor */
            if (distinct > 3) return false;   /* more than three factors */
        }
    }
    if (tmp > 1) ++distinct;              /* remaining prime factor */
    return distinct == 3;
}

/* Recursive generation of numbers with prime digits */
static void dfs_generate(int pos, int len, uint64_t cur,
                         idx_t target, idx_t *cnt,
                         seq_t *result, bool *found) {
    if (*found) return;
    if (pos == len) {
        if (is_sphenic(cur)) {
            ++(*cnt);
            if (*cnt == target) {
                *result = (seq_t)cur;
                *found   = true;
            }
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        uint64_t nxt = cur * 10 + DIGITS[i];
        dfs_generate(pos + 1, len, nxt, target, cnt, result, found);
        if (*found) return;
    }
}

seq_t a(idx_t n) {
    init_primes();
    idx_t cnt = 0;
    bool found = false;
    seq_t ans = 0;
    for (int len = 1; !found; ++len) {
        dfs_generate(0, len, 0, n, &cnt, &ans, &found);
    }
    return ans;
}
