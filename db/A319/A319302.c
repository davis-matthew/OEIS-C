/*
 * A319302  Integers whose binary representation contains a consecutive string
 *          of zeros of prime length.
 * https://oeis.org/A319302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose binary representation contains a
 *            consecutive run of zeros of prime length
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Prime lookup table up to 2048 bits */
static bool prime_table[2049];
static bool prime_table_inited = false;

static void init_prime_table(void)
{
    if (prime_table_inited) return;
    int limit = 2048;
    for (int i = 0; i <= limit; ++i) prime_table[i] = true;
    prime_table[0] = prime_table[1] = false;
    for (int i = 2; i * i <= limit; ++i) {
        if (prime_table[i]) {
            for (int j = i * i; j <= limit; j += i) {
                prime_table[j] = false;
            }
        }
    }
    prime_table_inited = true;
}

/* Cache for previously computed terms */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static seq_t next_candidate = 1;

static void ensure_capacity(size_t needed)
{
    if (seq_cache_cap >= needed) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* Test whether x has a run of zeros of prime length in its binary representation */
static bool has_prime_zero_run(seq_t x)
{
    unsigned run = 0;
    while (x > 0) {
        if ((x & 1) == 0) {
            ++run;
        } else {
            if (run > 0 && prime_table[run]) return true;
            run = 0;
        }
        x >>= 1;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_prime_table();

    if (n <= seq_cache_len) return seq_cache[n - 1];

    while (seq_cache_len < n) {
        seq_t candidate = next_candidate;
        ++next_candidate;

        if (has_prime_zero_run(candidate)) {
            ensure_capacity(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = candidate;
        }
    }
    return seq_cache[n - 1];
}
