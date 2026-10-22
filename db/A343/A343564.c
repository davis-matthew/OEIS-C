/*
 * A343564  a(n) is the sum of 2*n mod p for primes p such that 2*n-p is
 *          prime.
 * https://oeis.org/A343564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime, 2n-p prime} (2n mod p)
 * Range:     u8 n <= 41, u16 n <= 1049, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static idx_t _max_limit = 0;
static uint64_t *_prime_list = NULL;
static size_t _prime_cnt = 0;

static void ensure_primes_upto(idx_t limit) {
    if (limit <= _max_limit) return;
    uint64_t *p;
    size_t c;
    build_primes_upto(limit, &p, &c);
    if (_prime_list) free(_prime_list);
    _prime_list = p;
    _prime_cnt = c;
    _max_limit = limit;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t limit = 2 * n;
    ensure_primes_upto(limit);
    seq_t two_n = ((seq_t)n) * 2;
    seq_t sum = 0;
    for (size_t i = 0; i < _prime_cnt && _prime_list[i] <= limit; ++i) {
        idx_t p = _prime_list[i];
        idx_t q = limit - p;
        if (is_prime(q)) {
            sum += two_n % (seq_t)p;
        }
    }
    return sum;
}
