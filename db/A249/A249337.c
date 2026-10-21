/*
 * A249337  a(1) = 1, a(2) = 2; for n>2, a(n) = number of values k in range 1
 *          .. n-1 such that {sum of prime indices in the prime factorization
 *          of a(k)} = {sum of prime indices in the prime factorization of
 *          a(n-1)}, both counted with multiplicity.
 * https://oeis.org/A249337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 ≤ k ≤ n-1) such that A056239(a(k)) =
 *            A056239(a(n-1)), where A056239(n) is the sum of the prime
 *            indices (with multiplicity) in the prime factorisation of n
 * Range:     u8 n <= 2690, u16 n <= 8808, u32 n <= 8808, u64 n <= 8808,
 *            u128 n <= 8808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute the sum of prime indices (with multiplicity) of a positive integer
   (or 0 for n <= 1).  Index of 2 is 1, 3 is 2, 5 is 3, etc. */
static uint64_t prime_index(uint64_t p)
{
    if (p < 2) return 0;
    uint64_t idx = 0;
    for (uint64_t x = 2; x <= p; ++x) {
        bool isp = true;
        for (uint64_t d = 2; d*d <= x; ++d) {
            if (x % d == 0) { isp = false; break; }
        }
        if (isp) ++idx;
        if (x == p) break;
    }
    return idx;   /* 1‑based */
}

static uint64_t A056239_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t sum = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p*p <= tmp; ++p) {
        while (tmp % p == 0) {
            sum += prime_index(p);
            tmp /= p;
        }
    }
    if (tmp > 1) sum += prime_index(tmp);
    return sum;
}

/* Convert seq_t (compiled‐int or big int) to uint64_t (safe for small values) */
static uint64_t to_uint64(seq_t v)
{
    return (uint64_t)v;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t *freq_sums = NULL;
    static size_t *freq_counts = NULL;
    static size_t freq_cap = 0;
    static size_t freq_len = 0;

    /* Grow the sequence to at least n */
    while (seq_len < (size_t)n) {
        if (seq_len == 0) { /* initialise a(1) and a(2) on first call */
            seq_cap = 16;
            seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
            seq[0] = (seq_t)1;   /* a(1) */
            seq[1] = (seq_t)2;   /* a(2) */
            seq_len = 2;

            /* initialise frequency table */
            freq_cap = 8;
            freq_sums = (uint64_t*)malloc(freq_cap * sizeof(uint64_t));
            freq_counts = (size_t*)malloc(freq_cap * sizeof(size_t));
            freq_len = 0;
            /* add sums of a(1) and a(2) */
            auto_add_freq:
            for (size_t i = 0; i < seq_len; ++i) {
                uint64_t sum = A056239_uint64(to_uint64(seq[i]));
                /* add to freq */
                bool found = false;
                for (size_t j = 0; j < freq_len; ++j) {
                    if (freq_sums[j] == sum) {
                        ++freq_counts[j];
                        found = true; break;
                    }
                }
                if (!found) {
                    if (freq_len == freq_cap) {
                        freq_cap *= 2;
                        freq_sums = (uint64_t*)realloc(freq_sums, freq_cap * sizeof(uint64_t));
                        freq_counts = (size_t*)realloc(freq_counts, freq_cap * sizeof(size_t));
                    }
                    freq_sums[freq_len] = sum;
                    freq_counts[freq_len] = 1;
                    ++freq_len;
                }
            }
            continue;
        }

        /* Compute a(n) using existing sequence and frequency map */
        uint64_t prev_sum = A056239_uint64(to_uint64(seq[seq_len - 1]));
        /* fetch current frequency of prev_sum */
        size_t cnt = 0;
        for (size_t j = 0; j < freq_len; ++j) {
            if (freq_sums[j] == prev_sum) { cnt = freq_counts[j]; break; }
        }

        /* Append new value */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = (seq_t)cnt;
        ++seq_len;

        /* Add sum of new value to frequency table */
        uint64_t new_sum = A056239_uint64(to_uint64(seq[seq_len - 1]));
        bool found = false;
        for (size_t j = 0; j < freq_len; ++j) {
            if (freq_sums[j] == new_sum) {
                ++freq_counts[j];
                found = true; break;
            }
        }
        if (!found) {
            if (freq_len == freq_cap) {
                freq_cap *= 2;
                freq_sums = (uint64_t*)realloc(freq_sums, freq_cap * sizeof(uint64_t));
                freq_counts = (size_t*)realloc(freq_counts, freq_cap * sizeof(size_t));
            }
            freq_sums[freq_len] = new_sum;
            freq_counts[freq_len] = 1;
            ++freq_len;
        }
    }

    return seq[n - 1];
}
