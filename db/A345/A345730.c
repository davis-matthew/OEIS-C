/*
 * A345730  Variation on the Inventory Sequence A342585: record the number of
 *          occurrences of previous terms with an incrementing number of given
 *          divisors until 0 is recorded, then restart the divisor count from
 *          0. See the Comments.
 * https://oeis.org/A345730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined as the iterative inventory of divisor counts, as
 *            described in OEIS A345730
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Count the number of positive divisors of n.  For n==0 a special
   value 0 is returned, matching the definition in the sequence. */
static uint64_t count_divisors_u64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            result *= (exp + 1);
        }
        p++;
    }
    if (n > 1) result *= 2;
    return result;
}

/* The sequence generating function. */
seq_t a(idx_t n) {
    /* Static state to allow random‑ordered calls. */
    static seq_t      *seq     = NULL;
    static idx_t          seq_cap = 0;
    static idx_t          seq_len = 0;

    static uint64_t  *freq    = NULL;
    static size_t       freq_cap = 0;
    static int           cur_d   = 0; /* current divisor count to query */

    /* One‑time initialisation. */
    if (seq == NULL) {
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;                 /* a(0) = 0 */
        seq_len = 1;

        freq_cap = 8;
        freq = (uint64_t *)calloc(freq_cap, sizeof(uint64_t));
        freq[0] = 1;                 /* there is one zero (a(0)) */
        cur_d = 0;
    }

    /* Generate terms up to the requested index n. */
    while (seq_len <= n) {
        uint64_t count = 0;
        if ((size_t)cur_d < freq_cap) count = freq[cur_d];
        seq_t val = (seq_t)count;    /* store the next term */

        /* Append the new term. */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = val;

        /* Update frequency table with divisor count of the new term. */
        uint64_t val_div = (count == 0) ? 0 : count_divisors_u64(count);
        if (val_div >= freq_cap) {
            size_t new_cap = val_div + 1;
            uint64_t *tmp = (uint64_t *)realloc(freq, new_cap * sizeof(uint64_t));
            if (tmp == NULL) exit(1);            /* allocation failure */
            memset(tmp + freq_cap, 0, (new_cap - freq_cap) * sizeof(uint64_t));
            freq = tmp;
            freq_cap = new_cap;
        }
        freq[val_div]++;

        /* Prepare divisor count for the next term. */
        if (count == 0) {
            cur_d = 0;    /* restart a new run */
        } else {
            cur_d++;      /* continue incrementing */
        }
    }

    return seq[n];
}
