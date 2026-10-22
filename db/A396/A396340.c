/*
 * A396340  Primes prime(k) such that prime(k) - prime(k-1) is a Fibonacci
 *          number.
 * https://oeis.org/A396340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p_k such that prime(k)-prime(k-1) is a Fibonacci number */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* Helper: dynamic Fibonacci array up to a given maximum gap */
static uint64_t * _fib_arr = NULL;
static size_t _fib_cap = 0;
static size_t _fib_cnt = 0;

static void _add_fibs_up_to(uint64_t max_gap) {
    if (_fib_cnt == 0) {
        _fib_cap = 16;
        _fib_arr = (uint64_t*)malloc(_fib_cap * sizeof(uint64_t));
        _fib_arr[0] = 1;
        _fib_arr[1] = 1;
        _fib_cnt = 2;
    }
    while (_fib_arr[_fib_cnt - 1] < max_gap) {
        if (_fib_cnt == _fib_cap) {
            _fib_cap <<= 1;
            _fib_arr = (uint64_t*)realloc(_fib_arr, _fib_cap * sizeof(uint64_t));
        }
        uint64_t next = _fib_arr[_fib_cnt - 1] + _fib_arr[_fib_cnt - 2];
        _fib_arr[_fib_cnt++] = next;
        if (next >= max_gap) break;
    }
}

static bool _gap_is_fib(uint64_t gap) {
    _add_fibs_up_to(gap);
    size_t lo = 0, hi = _fib_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = _fib_arr[mid];
        if (v == gap) return true;
        if (v < gap) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Main function */
seq_t a(idx_t n) {
    /* Static cache for found terms */
    static seq_t *found = NULL;
    static size_t found_cap = 0;
    static size_t found_len = 0;

    /* State for prime generation */
    static bool initialized = false;
    static uint64_t prev_prime = 2;      /* start from 2, so next is 3 */
    static uint64_t curr_prime = 0;      /* will be set on first run */

    if (!initialized) {
        /* Prepare cache */
        found_cap = 16;
        found = (seq_t*)malloc(found_cap * sizeof(seq_t));
        found_len = 0;
        /* Initialize prime state */
        prev_prime = 2;
        curr_prime = 0;
        initialized = true;
    }

    /* Generate entries until we have at least n terms */
    while (found_len < n) {
        curr_prime = next_prime(prev_prime);          /* next prime after prev */
        uint64_t gap = curr_prime - prev_prime;       /* difference */

        if (_gap_is_fib(gap)) {
            if (found_len == found_cap) {
                found_cap <<= 1;
                found = (seq_t*)realloc(found, found_cap * sizeof(seq_t));
            }
            found[found_len++] = (seq_t)curr_prime;   /* store as seq_t */
        }
        prev_prime = curr_prime;
    }

    return found[n - 1];
}
