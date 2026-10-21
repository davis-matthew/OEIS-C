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
 * Helpers:   is_barker, pack_key, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_cmp.h"   /* comparator for seq_t values */
#include "../utils/is_barker.h"
#include "../utils/pack_key.h"

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

    qsort(seq_terms, seq_len, sizeof(seq_t), seq_cmp);
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > seq_len) return 0;
    return seq_terms[n - 1];
}
