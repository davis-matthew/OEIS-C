/*
 * A338779  a(n) is the smallest number k such that period of continued
 *          fraction for sqrt(prime(j)) equal for all prime(k) <= prime(j) <
 *          prime(k + n).
 * https://oeis.org/A338779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) is computed by scanning primes, computing the period of the
 *            continued fraction of sqrt(p), and finding the first run of
 *            equal periods of length at least n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations for utility functions */
seq_t floor_sqrt(seq_t);
seq_t _floor_isqrt_u64(idx_t);
seq_t seq_from_str(const char *s);

/* Helper: integer square root for 64‑bit unsigned integers */
static uint64_t isqrt64(uint64_t n) {
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;           /* largest power of four <= 2^64 */
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Compute the period of the simple continued fraction of sqrt(D) */
static uint64_t period_of_sqrt(uint64_t D) {
    uint64_t a0 = isqrt64(D);
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    uint64_t period = 0;
    do {
        m = (uint64_t)(((__uint128_t)d * a) - m);
        d = (uint64_t)((D - (__uint128_t)m * m) / d);
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);
    return period;
}

/* Prime generation using a growing list of known primes */
static uint64_t *prime_list = NULL;
static size_t prime_list_len = 0;
static size_t prime_list_cap = 0;

static uint64_t next_prime(uint64_t start) {
    if (prime_list_len == 0) {                 /* first call, seed list */
        prime_list_cap = 16;
        prime_list = (uint64_t *)malloc(prime_list_cap * sizeof(uint64_t));
        prime_list_len = 1;
        prime_list[0] = 2;
    }
    uint64_t cand = start + 1;
    if (cand <= 2) return 2;
    if (cand % 2 == 0) cand++;

    for (;;) {
        bool prime = true;
        uint64_t sq = (uint64_t) isqrt64(cand);
        for (size_t i = 0; i < prime_list_len && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (prime_list_len == prime_list_cap) {
                prime_list_cap = prime_list_cap ? prime_list_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_list_cap * sizeof(uint64_t));
                prime_list = tmp;
            }
            prime_list[prime_list_len++] = cand;
            return cand;
        }
        cand += 2;
    }
}

/* Storage for the sequence a(n) */
static seq_t *seq_a = NULL;
static size_t seq_cap = 0;
static idx_t a_count = 0;          /* highest n for which a(n) is known */

/* Current state of the scanning process */
static idx_t cur_index = 1;        /* index of the last processed prime */
static uint64_t cur_prime = 2;     /* last processed prime value */
static uint64_t run_start_index = 1;
static uint64_t run_value = 0;
static uint64_t run_length = 1;

/* Ensure seq_a has space for at least up to 'need' (inclusive) */
static void ensure_capacity(idx_t need) {
    if (need + 1 <= seq_cap) return;
    size_t new_cap = need + 1;
    if (new_cap < 16) new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(seq_a, new_cap * sizeof(seq_t));
    seq_a = tmp;
    seq_cap = new_cap;
}

/* Perform the scanning until all a(n) up to 'need' are known */
static void fill_until(idx_t need) {
    if (need <= a_count) return;

    ensure_capacity(need);

    /* Initialization on first call */
    if (a_count == 0) {
        seq_a[1] = (seq_t)1;       /* a(1) = 1 */
        a_count = 1;
        run_value = period_of_sqrt(2);   /* period for prime 2 */
        run_start_index = 1;
        run_length = 1;
    }

    /* Scan primes until we know all required terms */
    while (a_count < need) {
        cur_prime = next_prime(cur_prime);
        cur_index++;

        uint64_t period = period_of_sqrt(cur_prime);

        if (period == run_value) {
            run_length++;
        } else {
            /* close previous run */
            idx_t assign_start = a_count + 1;
            idx_t assign_end = run_length;
            if (assign_end > need) assign_end = need;
            for (idx_t k = assign_start; k <= assign_end; ++k) {
                seq_a[k] = (seq_t)run_start_index;
            }
            if (run_length > a_count) a_count = run_length;

            /* start new run */
            run_start_index = cur_index;
            run_value = period;
            run_length = 1;
        }
    }
}

seq_t a(idx_t n) {
    fill_until(n);
    return seq_a[n];
}
