/*
 * A373265  Numbers k such that k' / gcd(k,k') is of the form 4m+2, where k'
 *          stands for the arithmetic derivative of k, A003415.
 * https://oeis.org/A373265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, gcd, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers k such that k' / gcd(k,k') ≡ 2 (mod 4)
   (k' is the arithmetic derivative of k).
   The function searches forward from 2, testing each integer
   until the desired count is reached.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/seq_alloc_new_array.h"

static seq_t *seq = NULL;      /* cached sequence values       */
static size_t seq_len = 0;     /* number of stored terms       */
static size_t seq_cap = 0;     /* current array capacity      */
static uint64_t last_checked = 1;  /* last integer examined */

/* Append a new term to the cache.  */
static void append_seq(uint64_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *newp = seq_alloc_new_array(new_cap);
        if (!newp) exit(EXIT_FAILURE);
        if (seq) memcpy(newp, seq, seq_len * sizeof(seq_t));
        free(seq);
        seq = newp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* Test whether integer n satisfies the property.  */
static bool is_candidate(uint64_t n)
{
    seq_t deriv = 0;
    uint64_t tmp = n;
    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);
        uint64_t e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        deriv += (seq_t)n * (seq_t)e / (seq_t)p;
    }
    seq_t g = gcd(deriv, (seq_t)n);
    seq_t d = deriv / g;
    return (d % 4) == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t target = (size_t)n;      /* number of terms required */
    if (seq_len < target) {
        while (seq_len < target) {
            uint64_t cand = last_checked + 1;
            while (1) {
                if (is_candidate(cand)) {
                    append_seq(cand);
                    last_checked = cand;
                    break;
                }
                ++cand;
            }
        }
    }
    return seq[target - 1];
}
