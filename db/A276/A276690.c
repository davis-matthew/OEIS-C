/*
 * A276690  Binary Barker codes written in base 10.
 * https://oeis.org/A276690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Binary Barker codes (canonical representatives) in base
 *            10 The sequence is computed by enumerating all binary Barker
 *            sequences of lengths 2..13, grouping them by identical aperiodic
 *            autocorrelation arrays, keeping the largest decimal value of
 *            each class, and returning the terms sorted asce...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Pack autocorrelation array into a 32‑bit key.
   Each value is in {-1,0,1}. Encode as two bits: 00->-1, 01->0, 10->1. */
static inline uint32_t pack_key(const int *ac, int n)
{
    uint32_t k = 0;
    for (int i = 0; i < n; ++i) {
        k = (k << 2) | (ac[i] + 1);   /* ac[i]+1 in {0,1,2} */
    }
    return k;
}

/* Test if a binary word of length N is a Barker sequence.
   If yes, return the packed key via *key_out. */
static bool is_barker(uint64_t seq, uint8_t N, uint32_t *key_out)
{
    int bits[13];  /* N <= 13 */
    for (int i = 0; i < N; ++i)
        bits[i] = ((seq >> (N - 1 - i)) & 1ULL) ? 1 : -1;

    int ac[12]; /* N-1 <= 12 */
    for (int k = 1; k < N; ++k) {
        int sum = 0;
        for (int i = 0; i < N - k; ++i)
            sum += bits[i] * bits[i + k];
        if (sum > 1 || sum < -1) return false;
        ac[k - 1] = sum;
    }
    *key_out = pack_key(ac, N - 1);
    return true;
}

#define MAX_N 13            /* largest known binary Barker length */
#define MAX_BITS (1U << MAX_N)
#define MAX_CLASSES MAX_BITS /* upper bound on number of classes */

/* Structure to hold a class key and its maximum value */
typedef struct {
    uint8_t  len;          /* sequence length */
    uint32_t key;          /* packed autocorrelation pattern */
    seq_t    max;          /* maximum decimal value of this class */
} barker_class_t;

/* Static storage for the sequence terms */
static seq_t seq_terms[MAX_CLASSES];
static size_t seq_len = 0;          /* number of terms produced */

/* Cached classes */
static barker_class_t classes[MAX_CLASSES];
static size_t class_cnt = 0;

/* Locate class by len and key; return index or -1 if not found. */
static int find_class(uint8_t len, uint32_t key)
{
    for (size_t i = 0; i < class_cnt; ++i)
        if (classes[i].len == len && classes[i].key == key)
            return (int)i;
    return -1;
}

/* One-time initialization of the sequence. */
static void init_sequence(void)
{
    if (seq_len) return;  /* already initialized */

    for (uint8_t N = 2; N <= MAX_N; ++N) {
        uint64_t limit = (uint64_t)1 << N;
        for (uint64_t seq = 0; seq < limit; ++seq) {
            uint32_t key;
            if (!is_barker(seq, N, &key)) continue;

            int idx = find_class(N, key);
            if (idx < 0) {
                /* new class */
                classes[class_cnt].len  = N;
                classes[class_cnt].key  = key;
                classes[class_cnt].max  = (seq_t)seq;
                ++class_cnt;
            } else {
                if ((seq_t)seq > classes[idx].max)
                    classes[idx].max = (seq_t)seq;
            }
        }
    }

    /* collect maximal values and sort them ascending */
    seq_len = class_cnt;
    for (size_t i = 0; i < seq_len; ++i)
        seq_terms[i] = classes[i].max;

    mem_qsort(seq_terms, seq_len, sizeof(seq_t), seq_cmp);
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > seq_len) return 0;
    return seq_terms[n - 1];
}
