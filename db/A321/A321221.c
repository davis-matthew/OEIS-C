/*
 * A321221  Numbers of the form 6n-2 which are not a sum of two numbers that
 *          are the lesser of twin primes.
 * https://oeis.org/A321221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = numbers of the form 6k-2 that are not the sum of two
 *            lesser-of-twin primes. Computed by incremental sieving of primes
 *            up to the current candidate and marking all sums of twin primes.
 *            The nth term is returned
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The external types seq_t and idx_t are supplied by the build. */

static seq_t *seq_values = NULL;
static size_t seq_count = 0;
static size_t seq_cap   = 0;

static unsigned char *prime_table = NULL;
static unsigned char *rep_table   = NULL;
static idx_t current_limit = 0;

static uint32_t *twin_vals  = NULL;
static size_t twin_count = 0;
static size_t twin_cap   = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    while (seq_cap < needed) seq_cap = seq_cap ? seq_cap * 2 : 64;
    seq_values = (seq_t *)realloc(seq_values, seq_cap * sizeof(seq_t));
    if (!seq_values) exit(EXIT_FAILURE);
}

static void ensure_limit(idx_t new_limit)
{
    if (new_limit <= current_limit) return;

    free(prime_table);
    free(rep_table);
    free(twin_vals);

    prime_table = (unsigned char *)calloc((size_t)(new_limit + 1), sizeof(unsigned char));
    rep_table   = (unsigned char *)calloc((size_t)(new_limit + 1), sizeof(unsigned char));
    if (!prime_table || !rep_table) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= new_limit; ++i) prime_table[i] = 1;
    for (idx_t i = 2; i * i <= new_limit; ++i)
        if (prime_table[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                prime_table[j] = 0;

    twin_cap = 128;
    twin_vals = (uint32_t *)malloc(twin_cap * sizeof(uint32_t));
    twin_count = 0;
    for (idx_t i = 3; i + 2 <= new_limit; i += 2)
        if (prime_table[i] && prime_table[i + 2]) {
            if (twin_count == twin_cap) {
                twin_cap <<= 1;
                twin_vals = (uint32_t *)realloc(twin_vals, twin_cap * sizeof(uint32_t));
                if (!twin_vals) exit(EXIT_FAILURE);
            }
            twin_vals[twin_count++] = (uint32_t)i;
        }

    for (size_t a = 0; a < twin_count; ++a) {
        uint32_t pa = twin_vals[a];
        for (size_t b = a; b < twin_count; ++b) {
            uint32_t sum = pa + twin_vals[b];
            if (sum > new_limit) break;
            rep_table[sum] = 1;
        }
    }

    current_limit = new_limit;
}

seq_t a(idx_t n)
{
    static idx_t next_k = 1;

    while (seq_count < n) {
        idx_t m = 6 * next_k - 2;
        if (m > current_limit) ensure_limit(m);

        if (!rep_table[m]) {
            ensure_seq_capacity(seq_count + 1);
            seq_values[seq_count++] = (seq_t)m;
        }
        ++next_k;
    }
    return seq_values[n - 1];
}
